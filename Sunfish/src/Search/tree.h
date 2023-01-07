/*
 * tree.h
 *
 *  Created on: 2012/06/16
 *      Author: ryosuke
 */

#ifndef TREE_H_
#define TREE_H_

#include "../Evaluates/evaluate.h"
#include "../Records/hashStack.h"
#include "../Shek/shekTable.h"
#include "nodeStat.h"
#include "node.h"
#include <boost/algorithm/string.hpp>
#define BOOST_THREAD_USE_LIB
#include <boost/thread.hpp>

namespace Search {
	class Tree {
	private:
		boost::mutex mutex;
		Shogi::Position pos;
		Evaluates::Evaluate eval;
		Shek::ShekTable shekTable;
		History& history;
		Node* nodes;
		int depth;
		int maxDepth;

	public:
		// 並列探索用
		struct SPLIT {
			static const int TREE_NULL = -1;
			volatile int self; // 自分の番号
			volatile int parent; // 親の番号
			volatile int worker; // worker の番号
			volatile bool used; // 使用状態
			volatile int childCount; // 子供の tree の数
			volatile bool shutdown; // 直ちに終了
			volatile int depth; // 残り深さ
			Evaluates::Value alpha; // alpha値
			Evaluates::Value beta; // beta値
			NodeStat stat; // node status
			Evaluates::Value standPat; // stand pat
			volatile bool mate; // 詰めろの手
			volatile bool pvNode;
			Evaluates::Value value; // 暫定解 (workerが更新)
			Shogi::Move best; // 暫定解 (workerが更新)
		} split;

		static const int DEF_MAX_DEPTH = 64;

		Tree(const Evaluates::Param& param, History& history,
				int index = 0, int maxDepth = DEF_MAX_DEPTH) :
				eval(param), history(history),
				nodes(NULL) {
			init(maxDepth);
			split.self = index;
		}

		Tree(const Evaluates::Param& param,
				const Shogi::Position& pos, History& history,
				int index = 0, int maxDepth = DEF_MAX_DEPTH) :
				pos(pos), eval(param),
				history(history), nodes(NULL) {
			eval.init(pos);
			init(maxDepth);
			split.self = index;
		}

		virtual ~Tree() {
			if (nodes != NULL) {
				delete[] nodes;
			}
		}

		int getDepth() const {
			return depth;
		}

		int getMaxDepth() const {
			return maxDepth;
		}

		bool isMaxDepth() {
			return depth == maxDepth;
		}

		void init(int maxDepth = DEF_MAX_DEPTH) {
			if (nodes != NULL) {
				delete[] nodes;
			}
			this->maxDepth = maxDepth;
			nodes = new Node[maxDepth+1];
			for (int i = 0; i < maxDepth; i++) {
				nodes[i].init(pos, eval.getParam(), history);
			}
			depth = 0;
		}

		void init(const Shogi::Position& pos, int maxDepth = DEF_MAX_DEPTH) {
			this->pos.copy(pos);
			eval.init(this->pos);
			init(maxDepth);
		}

		void initNode() {
			nodes[depth].arrive(pos.getHash(), _isCheck());
			nodes[depth+1].initKiller();
		}

		int updatePv() {
			return nodes[depth].setPv(nodes[depth+1]);
		}

		int updatePv(const Shogi::Move& move, const Tree& child) {
			return nodes[depth].setPv(move, child.nodes[depth+1]);
		}

		int setPv(const Shogi::Move& move) {
			return nodes[depth].setPv(move);
		}

		const Pv& getRootPv() const {
			return getPv(0);
		}

		const Pv& getPv() const {
			return getPv(depth);
		}

		const Pv& getInnerPv() const {
			return getPv(depth+1);
		}

		const Pv& getPv(int depth) const {
			return nodes[depth].getPv();
		}

		void generateMovesAtOnce() {
			nodes[depth].generateMovesAtOnce();
		}

		void generateMoves() {
			nodes[depth].generateMoves();
		}

		void generateTacticalMoves() {
			nodes[depth].generateTacticalMoves();
		}

		void generateCaptures() {
			nodes[depth].generateCaptures();
		}

		void generateCheck() {
			nodes[depth].generateCheck();
		}

		bool next() {
			return nodes[depth].next();
		}

		bool makeMove(bool shek, bool doEval = true) {
			if (depth < maxDepth) {
				nodes[depth].setShek(shek);
				if (shek) { shekSet(); }
				nodes[depth++].makeMove(pos, eval, doEval);
				return true;
			}
			return false;
		}

		bool nullMove(bool shek) {
			if (depth < maxDepth) {
				nodes[depth].setShek(shek);
				if (shek) { shekSet(); }
				if (nodes[depth].nullMove(pos, eval)) {
					depth++;
					return true;
				}
				if (shek) { shekUnset(); }
			}
			return false;
		}

		void unmakeMove() {
			nodes[--depth].unmakeMove(pos, eval);
			if (nodes[depth].isShek()) { shekUnset(); }
		}

		const Shogi::Move* getPrevMove() const {
			return depth > 0 ? nodes[depth-1].getMove() : NULL;
		}

		const Shogi::Move* getCurrentMove() const {
			return nodes[depth].getMove();
		}

		const Shogi::Move& getMove(int index) const {
			return nodes[depth].getMove(index);
		}

		void setMove(const Shogi::Move& move) {
			nodes[depth].setMove(&move);
		}
 
#ifndef NDEBUG
		void debugPrint() const {
			Log::debug << debugString() << '\n';
		}
#endif

		std::string debugString() const {
			std::ostringstream oss;
			for (int i = 0; i < depth; i++) {
				const Shogi::Move* pmove = nodes[i].getMove();
				if (pmove != NULL) {
					oss << pmove->toString() << ' ';
				} else {
					oss << "null ";
				}
			}
			return oss.str();
		}

		const Shogi::Change* getChange() const {
			return depth > 0 ? &nodes[depth-1].getChange() : NULL;
		}

		const Shogi::Position& getPosition() const {
			return pos;
		}

		bool isMate() const {
			return pos.isMate();
		}

		Util::uint64 getHash(int depth) const {
			return nodes[depth].getHash();
		}

		bool _isCheck() const {
			return pos.isCheck();
		}

		bool isCheck() const {
			return nodes[depth].isCheck();
		}

		bool isCheck(int depth) const {
			return nodes[depth].isCheck();
		}

		bool isEvasion() const {
			return depth >= 1 && nodes[depth-1].isCheck();
		}

		bool isCheckMove() const {
			const Shogi::Move* pmove = getCurrentMove();
			if (pmove != NULL) {
				return pos.isCheckMove(*pmove);
			}
			return false;
		}

		bool isCapture() const {
			const Shogi::Move* pmove = getCurrentMove();
			if (pmove != NULL) {
				return pos.isCapturingMove(*pmove);
			}
			return false;
		}

		bool isRecapture() const {
			const Shogi::Move* pmove;
			const Shogi::Move* pprev;
			if ((pmove = getCurrentMove()) != NULL &&
					(pprev = getPrevMove()) != NULL) {
				return pmove->getTo() == pprev->getTo();
			}
			return false;
		}

		bool isTacticalMove() const {
			const Shogi::Move* pmove = getCurrentMove();
			if (pmove != NULL) {
				return (pmove->isPromotion()
						&& !pmove->getPiece().getTurnedBlack()
								.is(Shogi::Piece::BSILVER))
						|| pos.isCapturingMove(*pmove);
			}
			return false;
		}

		bool isOneReply() {
			return isCheck() && nodes[depth].getNumberOfMoves() == 1;
		}

		Evaluates::Value evaluate() {
			return eval.getValue();
		}

		Evaluates::Value negaEvaluate() {
			if (pos.isBlackTurn()) {
				return eval.getValue();
			} else {
				return -eval.getValue();
			}
		}

		Evaluates::Value estimate() const {
			const Shogi::Move* pmove = getCurrentMove();
			if (pmove != NULL) {
				return eval.estimate(pos, *pmove);
			}
			return Evaluates::Value(0);
		}

		Evaluates::Value negaEstimate() const {
			return pos.isBlackTurn() ? estimate() : -estimate();
		}

		void setHashMove(const HashMove& hashMove) {
			return nodes[depth].setHashMove(hashMove);
		}

		bool isHashMove() const {
			return nodes[depth].isHashMove();
		}

		void addKiller(Evaluates::Value value) const {
			const Shogi::Move* pmove = getCurrentMove();
			if (pmove != NULL) {
				value -= Killer::calcCurrentChange(pos, *pmove, eval.getParam());
				nodes[depth].addKiller(*pmove, value);
			}
		}

		void setMoveIndex(int index) {
			nodes[depth].setMoveIndex(index);
		}

		int getMoveIndex() const {
			return nodes[depth].getMoveIndex();
		}

		int getNumberOfMoves() const {
			return nodes[depth].getNumberOfMoves();
		}

		int getPrevNumberOfMoves() const {
			return nodes[depth-1].getNumberOfMoves();
		}

		int getNumberOfMoves(int depth) const {
			return nodes[depth].getNumberOfMoves();
		}

		void addHistory(int depth) const {
			nodes[this->depth].addHistory(history, depth);
		}

		void addHistory(int depth, int index) const {
			nodes[this->depth].addHistory(history, depth, index);
		}

		unsigned getHistory() const {
			return history.get(*getCurrentMove());
		}

		unsigned getHistory(const Shogi::Move& move) const {
			return history.get(move);
		}

		void sort(Evaluates::Value values[]) {
			nodes[0].sort(values);
		}

		std::string toString() const {
			return pos.toString();
		}

		bool is(const char* route) const {
			std::vector<std::string> tokens;
			boost::algorithm::split(tokens, route, boost::is_any_of(" "));
			if (tokens.size() != (unsigned)depth) {
				return false;
			}
			for (int i = 0; i < depth; i++) {
				if (nodes[i].isNullMove()) {
					if (tokens[i] != "null") {
						return false;
					}
				} else {
					const Shogi::Move* pmove = nodes[i].getMove();
					if (pmove == NULL || tokens[i] != pmove->toStringCsa()){
						return false;
					}
				}
			}
			return true;
		}

		bool startWith(const char* route) const {
			std::vector<std::string> tokens;
			boost::algorithm::split(tokens, route, boost::is_any_of(" "));
			if (tokens.size() > (unsigned)depth) {
				return false;
			}
			for (unsigned i = 0; i < tokens.size(); i++) {
				if (nodes[i].isNullMove()) {
					if (tokens[i] != "null") {
						return false;
					}
				} else {
					const Shogi::Move* pmove = nodes[i].getMove();
					if (pmove == NULL || tokens[i] != pmove->toStringCsa()){
						return false;
					}
				}
			}
			return true;
		}

		Killer& getKiller() {
			return nodes[depth].getKiller();
		}

		Killer& getInnerKiller() {
			return nodes[depth+1].getKiller();
		}

		void shekSet(const Records::HashStack& hashStack) {
			shekTable.set(hashStack);
		}

		void shekUnset(const Records::HashStack& hashStack) {
			shekTable.unset(hashStack);
		}

		void shekSet() {
			shekTable.set(pos);
		}

		void shekUnset() {
			shekTable.unset(pos);
		}

		Shek::ShekStat shekCheck() const {
			return shekTable.check(pos);
		}

#ifndef NDEBUG
		void shekDebug() const {
			shekTable.debugPrint(pos);
		}
#endif

		// split するときに親 tree へ情報をセット
		void setParentInfo(int depth,
				Evaluates::Value alpha,
				Evaluates::Value beta,
				Evaluates::Value value,
				NodeStat stat,
				Evaluates::Value standPat,
				bool mate,
				bool pvNode,
				int childCount) {
			split.childCount = childCount;
			split.depth = depth;
			split.alpha = alpha;
			split.beta = beta;
			split.stat = stat;
			split.standPat = standPat;
			split.mate = mate;
			split.pvNode = pvNode;
			split.value = value;
			split.best.setEmpty();
		}

		// split した時の子 tree に対して呼ぶ
		void use(Tree& tree, int worker) {
			fastCopy(tree);
			split.parent = tree.split.self;
			split.worker = worker;
			split.used = true;
			split.shutdown = false;
		}

		// root 局面が一致している場合に高速に同じ局面へ遷移する。
		void fastCopy(Tree& tree);

		// メインスレッドの場合
		void use(int worker) {
			split.parent = SPLIT::TREE_NULL;
			split.worker = worker;
			split.used = true;
			split.shutdown = false;
		}

		// tree の解放
		void unuse() {
			split.used = false;
		}

		boost::mutex& getMutex() {
			return mutex;
		}
	};
}

#endif // TREE_H_
