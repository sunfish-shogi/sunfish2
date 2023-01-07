/*
 * searcher.h
 *
 *  Created on: 2012/07/03
 *      Author: ryosuke
 */

#ifndef SEARCHER_H_
#define SEARCHER_H_

#include "../Util/timer.h"
#include "../Table/tt.h"
#include "../Shek/shekTable.h"
#include "../Records/record.h"
#include "pvHandler.h"
#include "tree.h"
#include "worker.h"
#include <vector>
#include <algorithm>
#define BOOST_THREAD_USE_LIB
#include <boost/thread.hpp>

namespace Search {
	struct SearchConfig {
		unsigned depth;
		PvHandler* pvHandler;
		bool limitEnable;
		double limitSeconds;
#ifndef NLEARN
		bool isLearning;
#endif // NLEARN
		static SearchConfig getDefault() {
			SearchConfig config = {32, NULL, false, .0
#ifndef NLEARN
				, false
#endif // NLEARN
				};
			return config;
		}
	};

	struct SearchResult {
		bool resign;
		Shogi::Move move;
		Pv pv;
		Evaluates::Value value;
		SearchCounter counter;
		std::vector<SearchCounter> counters;
		double sec;
		double nps;

		std::string toString() const;
	};

	class Searcher {
	private:
		// mutex
		boost::mutex splitMutex;
		boost::mutex flagMutex;

		static const int DEFAULT_WORKER_SIZE = 1;
		static const int NON_TREE_SIZE = 0;
		Tree* trees;
		int treeSize;
		volatile int idleTree;
		Worker* workers;
		int workerSize;
		volatile int idleWorker;

		Table::TT tt;
		History history;
		SearchConfig config;
		Util::Timer timer;
		int rootDepth;
		Evaluates::Value gain[Shogi::Piece::WDRAGON+1][Shogi::Square::END+1];
		Records::HashStack hashStack;

		//flags 
		volatile bool running;
		volatile bool signalInterrupt;
		volatile bool signalForceInterrupt;

		Evaluates::Value quies(Tree& tree, int ply,
				Evaluates::Value alpha,
				Evaluates::Value beta);

		bool isMate1Ply(Tree& tree);

		template <bool pvNode>
		Evaluates::Value negaMax(Tree& tree, int depth,
				Evaluates::Value alpha,
				Evaluates::Value beta,
				NodeStat stat = NodeStat());

		bool split(Tree& tree, int depth,
				Evaluates::Value alpha,
				Evaluates::Value beta,
				Evaluates::Value value,
				NodeStat stat,
				Evaluates::Value standPat,
				bool mate, bool pvNode);

		void shutdownTree(Tree& tree);

		void before(SearchResult& result);

		bool after(SearchResult& result, Evaluates::Value value);

		void totalCount(SearchCounter& counter) const;

		void updateGain(const Shogi::Move& move,
				const Evaluates::Value& before, const Evaluates::Value& after) {
			updateGain(move.getPiece(), move.getTo(), before, after);
		}

		void updateGain(const Shogi::Piece& piece, const Shogi::Square& square,
				const Evaluates::Value& before, const Evaluates::Value& after) {
			assert(piece <= Shogi::Piece::WDRAGON);
			assert(square <= Shogi::Square::END);
			gain[piece][square] = std::max(-(before + after), gain[piece][square] - 1);
		}

		Evaluates::Value getGain(const Shogi::Move& move) const {
			return getGain(move.getPiece(), move.getTo());
		}

		Evaluates::Value getGain(const Shogi::Piece& piece, const Shogi::Square& square) const {
			assert((unsigned)piece <= Shogi::Piece::WDRAGON);
			assert((unsigned)square <= Shogi::Square::END);
			return gain[piece][square];
		}

		bool isInterrupted(Tree& tree) const {
			if (signalForceInterrupt) {
				return true;
			} else if (tree.split.parent != Tree::SPLIT::TREE_NULL
					&& tree.split.shutdown) {
				return true;
			} else if (trees[0].getRootPv().getTop() != NULL) {
				if (signalInterrupt) {
					return true;
				} else if (config.limitEnable &&
						timer.get() >= config.limitSeconds) {
					return true;
				}
			}
			return false;
		}

		Shek::ShekStat shekCheck(Tree& tree) const {
			return tree.shekCheck();
		}

#ifndef NDEBUG
		void shekDebug(Tree& tree) const {
			tree.shekDebug();
		}
#endif

		bool nullMove(Tree& tree, bool shek = true) {
			return tree.nullMove(shek);
		}

		void makeMove(Tree& tree, bool shek = true,
				bool doEval = true) {
			tree.makeMove(shek, doEval);
			// FIXME: スタックがいっぱいで失敗するケース
		}

		void unmakeMove(Tree& tree) {
			tree.unmakeMove();
		}

		static int genTreeSize(int workerSize, int treeSize) {
			return treeSize != NON_TREE_SIZE ? treeSize : (workerSize * 4 - 3);
		}

		void buildWorkers() {
			workers = new Worker[workerSize];
			for (int i = 0; i < workerSize; i++) {
				workers[i].init(this, i);
			}
		}

		void buildInstances(const Evaluates::Param& param,
				History& history) {
			trees = (Tree*)new char[sizeof(Tree) * treeSize];
			for (int i = 0; i < treeSize; i++) {
				new (trees + i) Tree(param, history, i);
			}
			buildWorkers();
		}

		void buildInstances(const Evaluates::Param& param,
				const Shogi::Position& pos, History& history) {
			trees = (Tree*)new char[sizeof(Tree) * treeSize];
			for (int i = 0; i < treeSize; i++) {
				new (trees + i) Tree(param, pos, history);
			}
			buildWorkers();
		}

		void releaseInstances() {
			for (int i = 0; i < treeSize; i++) {
				trees[i].~Tree();
			}
			delete [] (char*)trees;
			delete [] workers;
		}

		enum REP_TYPE {
			REP_MY_CHECK,
			REP_EN_CHECK,
			REP_NORMAL,
		};

		REP_TYPE repType(const Tree& tree) const;

		static int recursiveDepth(int depth);

	public:
		static const int PLY1 = 4;

		Searcher(const Evaluates::Param& param,
				int workerSize = DEFAULT_WORKER_SIZE,
				int treeSize = NON_TREE_SIZE) :
				treeSize(genTreeSize(workerSize, treeSize)),
				workerSize(workerSize),
				hashStack(Records::HashStack::nan()),
				running(false),
				signalInterrupt(false),
				signalForceInterrupt(false) {
			buildInstances(param, history);
		}

		Searcher(const Evaluates::Param& param,
				const Shogi::Position& pos,
				int workerSize = DEFAULT_WORKER_SIZE,
				int treeSize = NON_TREE_SIZE) :
				treeSize(genTreeSize(workerSize, treeSize)),
				workerSize(workerSize),
				hashStack(Records::HashStack::nan()),
				running(false),
				signalInterrupt(false),
				signalForceInterrupt(false) {
			buildInstances(param, pos, history);
		}

		virtual ~Searcher() {
			releaseInstances();
		}

		void init(const Shogi::Position& pos,
				const Records::HashStack& hashStack) {
			for (int i = 0; i < treeSize; i++) {
				trees[i].init(pos);
			}
			this->hashStack = hashStack;
		}

		void init(const Records::Record& record) {
			init(record.getPosition(), record.getHashStack());
		}

		const SearchConfig& getSearchConfig() const {
			return config;
		}

		void setSearchConfig(const SearchConfig& config) {
			this->config = config;
		}

		bool isMate1Ply(const Shogi::Position& pos) {
			trees[0].init(pos);
			return isMate1Ply(trees[0]);
		}

		bool isMate1Ply() {
			return isMate1Ply(trees[0]);
		}

		bool search(SearchResult& result,
				Evaluates::Value alpha = Evaluates::Value::MIN,
				Evaluates::Value beta = Evaluates::Value::MAX);

		boost::mutex& getSplitMutex() {
			return splitMutex;
		}

		// iterative-deepening search
		bool idSearch(SearchResult& result);

		bool interrupt();

		bool forceInterrupt();

		bool isRunning() {
			return running;
		}

		void searchChildTree(int index) {
			searchChildTree(trees[index]);
		}

		void searchChildTree(Tree& tree);

		void releaseTree(int index) {
			trees[index].unuse();
			idleTree++;
			int parent = trees[index].split.parent;
			assert(parent != Tree::SPLIT::TREE_NULL);
			trees[parent].split.childCount--;
		}

#ifndef NDEBUG
		void debugPrintSplit() {
			for (int i = 0; i < treeSize; i++) {
				Log::debug << 't' << i << '\t'
						<< trees[i].split.self << '\t'
						<< trees[i].split.parent << '\t'
						<< trees[i].split.childCount << '\t'
						<< trees[i].split.worker << '\t'
						<< trees[i].split.used << '\n';
			}
			for (int i = 0; i < workerSize; i++) {
				Log::debug << 'w' << i << '\t'
						<< workers[i].hasJob() << '\n';
			}
		}
#endif

		void addIdleWorker() {
			idleWorker++;
		}

		void reduceIdleWorker() {
			idleWorker--;
		}

	private:
		class FutMgn {
		public:
			static const int MAX_DEPTH = 128;
			static const int MAX_COUNT = 128;
			int futMgn[MAX_DEPTH][MAX_COUNT];
			FutMgn();
		};
		static const FutMgn futMgn;

		static Evaluates::Value getFutMgn(int depth, int count = 0) {
			count = depth < 0 ? 0 : (depth < FutMgn::MAX_DEPTH ? depth : FutMgn::MAX_DEPTH-1);
			count = count < 0 ? 0 : (count < FutMgn::MAX_COUNT ? count : FutMgn::MAX_COUNT-1);
			return futMgn.futMgn[depth][count];
		}

		class NodeController {
		private:
			Searcher& searcher;
			Tree& tree;
			const Shogi::Move move;
			const NodeStat stat;
			NodeStat newStat;
			int depth;
			const Evaluates::Value alpha;
			const Evaluates::Value standPat;
			const Evaluates::Value estimate;
			int reduction;
			bool pruning;
			const unsigned moveCount;
			const bool _isNullWindow;
			const bool mate;
			const bool _isHash;
			const bool _isCheckMove;
			const bool _isTacticalMove;
			const bool _isCapture;
			const bool _isRecapture;
#ifdef PRUN_EXPR
			bool _isFut;
			bool _isExtFut;
			bool _isCount;
#endif
			Evaluates::Value newStandPat;

			template <bool isRoot>
			void execute();

		public:
			NodeController(Searcher& searcher, Tree& parent,
					Tree& tree, const NodeStat& stat, int depth,
					Evaluates::Value alpha, Evaluates::Value standPat,
					bool isNullWindow, bool mate) :
					searcher(searcher), tree(tree),
					move(*parent.getCurrentMove()),
					stat(stat), depth(depth),
					alpha(alpha), standPat(standPat),
					estimate(parent.negaEstimate()), reduction(0),
					pruning(false), moveCount(parent.getMoveIndex()),
					_isNullWindow(isNullWindow), mate(mate),
					_isHash(parent.isHashMove()),
					_isCheckMove(parent.isCheckMove()),
					_isTacticalMove(parent.isTacticalMove()),
					_isCapture(parent.isRecapture()),
					_isRecapture(parent.isRecapture())
#ifdef PRUN_EXPR
					,
					_isFut(false),
					_isExtFut(false),
					_isCount(false)
#endif
					{
			}

			void execute(bool isRoot = false) {
				if (isRoot) {
					execute<true>();
				} else {
					execute<false>();
				}
			}

			void executeInterior();

			const Shogi::Move& getMove() const {
				return move;
			}

			bool isPruning() const {
				return pruning;
			}

			int getDepth(bool reduced = true) const {
				return depth - (reduced ? reduction : 0);
			}

			int getReduction() const {
				return reduction;
			}

			bool isReduced() const {
				return reduction != 0;
			}

			bool isNullWindow() const {
				return _isNullWindow;
			}

			bool isCheckMove() const {
				return _isCheckMove;
			}

			bool isTacticalMove() const {
				return _isTacticalMove;
			}

			bool isCapture() const {
				return _isCapture;
			}

			bool isRecapture() const {
				return _isRecapture;
			}

			bool isHash () const {
				return _isHash;
			}

			bool isMateThreat() const {
				return mate;
			}

			unsigned getMoveCount() const {
				return moveCount;
			}

			Evaluates::Value getStandPat() const {
				return standPat;
			}

			void setNewStandPat(Evaluates::Value newStandPat) {
				this->newStandPat = newStandPat;
			}

			Evaluates::Value getNewStandPat() const {
				return newStandPat;
			}

			NodeStat getStat() const {
				return newStat;
			}

			const Evaluates::Value& getEstimate() const {
				return estimate;
			}

			static bool connectedThreat(Tree& tree,
					const Shogi::Move& threat,
					const Shogi::Move& move);

#ifdef PRUN_EXPR
			bool isFut() const {
				return _isFut;
			}

			bool isExtFut() const {
				return _isExtFut;
			}

			bool isCount() const {
				return _isCount;
			}
#endif
		};
	};
}

#endif // SEARCHER_H_
