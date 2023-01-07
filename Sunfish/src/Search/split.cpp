/*
 * split.cpp
 *
 *  Created on: 2013/02/18
 *      Author: ryosuke
 */

#include "searcher.h"
#include <memory>

namespace Search {
	using namespace Shogi;
	using namespace Evaluates;

	bool Searcher::split(Tree& tree, int depth,
			Value alpha, Value beta, Value value,
			NodeStat stat, Value standPat,
			bool mate, bool pvNode) {
		Worker& worker = workers[tree.split.worker];
		int myTree = Tree::SPLIT::TREE_NULL;
		{
			boost::mutex::scoped_lock lock(splitMutex);

			// 空き tree, worker の数を確認
			if (idleTree <= 1 || idleWorker <= 0) {
				return false;
			}

			// カレントスレッドに当てる tree を決定
			for (int i = 1; i < treeSize; i++) {
				if (!trees[i].split.used) {
					myTree = i;
					trees[i].use(tree, tree.split.worker);
					idleTree--;
					break;
				}
			}
			assert(myTree != Tree::SPLIT::TREE_NULL);

			// 他の worker と tree を確保
			int childCount = 1;
			for (int i = 0; i < workerSize; i++) {
				if (!workers[i].hasJob()) {
					for (int j = 1; j < treeSize; j++) {
						if (!trees[j].split.used) {
							// tree のセット
							trees[j].use(tree, i);
							idleTree--;
							// worker への job 割り当て
							workers[i].setJob(j);
							idleWorker--;

							childCount++;

							if (idleTree == 0 || idleWorker == 0) {
								goto lab_end_asign;
							}
							break;
						}
					}
				}
			}
			lab_end_asign:
				;

			// 親 tree に情報をセット
			tree.setParentInfo(depth, alpha, beta, value,
					stat, standPat, mate, pvNode, childCount);
			assert(childCount >= 2);
		}

		// カレントスレッドも job の処理に入る。
		worker.setTree(myTree);
		searchChildTree(trees[myTree]);
		worker.setTree(tree.split.self);

		// tree の解放
		{
			boost::mutex::scoped_lock lock(splitMutex);
			releaseTree(myTree);
		}

		if (!trees[myTree].split.shutdown) {
			// 兄弟の終了を待つあいだに他の job を拾う。
			worker.waitForJob(&tree);
		} else {
			// beta-cut の場合は単に兄弟の終了を待つ。
			while (true) {
				{
					//boost::mutex::scoped_lock lock(splitMutex);
					if (tree.split.childCount == 0) {
						break;
					}
				}
				boost::thread::yield();
			}
		}

		return true;
	}

	void Searcher::searchChildTree(Tree& tree) {
		{
			// バリア同期
			boost::mutex::scoped_lock lock(splitMutex);
		}
		Tree& parent = trees[tree.split.parent];
		Worker& worker = workers[tree.split.worker];
		SearchCounter& counter = worker.getCounter();
		int depth = parent.split.depth;
		Value alpha = parent.split.alpha;
		Value beta = parent.split.beta;
		NodeStat stat = parent.split.stat;
		Value standPat = parent.split.standPat;
		bool pvNode = parent.split.pvNode;

		while (true) {
			// 未展開の指し手を取得
			Move move;
			int moveCount;
			Value newAlpha;
			std::auto_ptr<NodeController> pnode; // TODO: 使わないで済ましたい。
			{
				boost::mutex::scoped_lock lock(parent.getMutex());
				// 次の指し手を取得
				if (!parent.next()) {
					// 指し手が無くなったら終了
					return;
				}
				move = *parent.getCurrentMove();
				moveCount = parent.getMoveIndex();
				// 新しい alpha 値
				newAlpha = Value::max(alpha, parent.split.value);
				// 現在の tree に次の指し手をセット
				tree.setMove(move);
				// node controller
				pnode.reset(new NodeController(*this, parent, tree,
						stat, depth - PLY1, newAlpha, standPat,
						beta == alpha + 1, parent.split.mate));
			}
			NodeController& node = *pnode.get();
			node.execute();

			if (node.isPruning()) {
				{
					boost::mutex::scoped_lock lock(parent.getMutex());
					parent.split.value = Value::max(parent.split.value, newAlpha);
				}
				counter.futilityPruning++;
				continue;
			}

			// make move
			Value newValue;
			makeMove(tree);

			// new stand-pat
			node.setNewStandPat(tree.negaEvaluate());
			node.executeInterior();

			if (node.isPruning()) {
				unmakeMove(tree);
				{
					boost::mutex::scoped_lock lock(parent.getMutex());
					parent.split.value = Value::max(parent.split.value, newAlpha);
				}
				counter.exFutilityPruning++;
				continue;
			}

			updateGain(node.getMove(), standPat + node.getEstimate(),
					node.getNewStandPat());

			// nega-scout
			newValue = -negaMax<false>(tree, node.getDepth(), -newAlpha-1, -newAlpha, node.getStat());
			if (!isInterrupted(tree) && newValue > newAlpha
					&& node.isReduced() != 0) {
				// reductionをなくして再探索
				newValue = -negaMax<false>(tree,
						node.getDepth(false),
						-newAlpha-1, -newAlpha, node.getStat());
			}
			if (!isInterrupted(tree) && newValue > newAlpha && !node.isNullWindow()) {
				// windowを広げて再探索
				if (pvNode) {
					newValue = -negaMax<true>(tree,
							node.getDepth(false),
							-beta, -newAlpha, node.getStat());
				} else {
					newValue = -negaMax<false>(tree,
							node.getDepth(false),
							-beta, -newAlpha, node.getStat());
				}
			}

			// unmake move
			unmakeMove(tree);

			if (isInterrupted(tree)) {
				return;
			}

			// 暫定解の更新
			{
				boost::mutex::scoped_lock lock(parent.getMutex());
				// 兄弟が beta-cut を起こしていたら直ちに終了
				if (tree.split.shutdown) {
					return;
				}
				if (newValue > parent.split.value) {
					parent.split.value = newValue;
					if (newValue > alpha) {
						parent.updatePv(move, tree);
						parent.split.best = move;
					}

					// beta cut
					if (newValue >= beta) {
						if (!node.isCapture()) {
							parent.addHistory(depth, moveCount);
							parent.addKiller(newValue - standPat);
						} else if (!node.isHash()) {
							parent.addKiller(newValue - standPat);
						}
						// 兄弟ノードの終了
						shutdownTree(parent);
						return;
					}
				}
			}
		}
	}

	void Searcher::shutdownTree(Tree& tree) {
		boost::mutex::scoped_lock lock(splitMutex);
		for (int i = 1; i < treeSize; i++) {
			int j = trees[i].split.parent;
			for (; j != Tree::SPLIT::TREE_NULL; j = trees[j].split.parent) {
				if (j == tree.split.self) {
					trees[i].split.shutdown = true;
					break;
				}
			}
		}
	}
}
