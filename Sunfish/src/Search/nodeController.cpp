/*
 * nodeController.cpp
 *
 *  Created on: 2013/02/12
 *      Author: ryosuke
 */

#include "searcher.h"

namespace Search {
	using namespace Shogi;
	using namespace Evaluates;

	template <bool isRoot>
	void Searcher::NodeController::execute() {
		// extensions
		if (isCheckMove()) {
			depth += PLY1;
		} else if (tree.isOneReply()) {
			depth += PLY1 * 2 / 4;
		} else if (isMateThreat()) {
			depth += PLY1 * 2 / 4;
		} else if (stat.isRecapture() && isRecapture()) {
			depth += PLY1 * 1 / 4;
			newStat.unsetRecapture();
		}

		if (!isHash() && moveCount != 1 && !isMateThreat()
				&& !tree.isCheck() && !isCheckMove()
				&& !isTacticalMove()) {
			// late move reduction
			if (depth >= (isRoot ? 2 * PLY1: PLY1)) {
				unsigned hist = tree.getHistory(move);
				if (!isNullWindow()) {
					if        (hist * 20U < History::SCALE) {
						reduction = Searcher::PLY1 * 3 / 2;
					} else if (hist *  6U < History::SCALE) {
						reduction = Searcher::PLY1;
					} else if (hist *  2U < History::SCALE) {
						reduction = Searcher::PLY1 / 2;
					}
				} else {
					if        (hist * 10U < History::SCALE) {
						reduction = Searcher::PLY1 * 2;
					} else if (hist *  6U < History::SCALE) {
						reduction = Searcher::PLY1 * 3 / 2;
					} else if (hist *  4U < History::SCALE) {
						reduction = Searcher::PLY1;
					} else if (hist *  1U < History::SCALE) {
						reduction = Searcher::PLY1 / 2;
					}
				}
			}

			if (!isRoot) {
				// futility pruning

#ifdef PRUN_EXPR
				_isCount = true;
#else
				// move count based pruning
				Killer& killer = tree.getInnerKiller();
				if (moveCount > 12U + depth / PLY1
						&& !connectedThreat(tree, killer.get1(), move)
						&& !connectedThreat(tree, killer.get2(), move)) {
					return;
				}
#endif

#ifdef PRUN_EXPR
				_isFut = true;
#else
				// value based pruning
				if (standPat + estimate
						+ Searcher::getFutMgn(depth - reduction, moveCount)
						+ searcher.getGain(move) <= alpha) {
					pruning = true;
					return;
				}
#endif
			}
		}
	}
	template void Searcher::NodeController::execute<true>();
	template void Searcher::NodeController::execute<false>();

	void Searcher::NodeController::executeInterior() {
		// extended futility pruning
		if (!isHash() && !isMateThreat()
				&& !isCheckMove() && !isTacticalMove()) {
#ifdef PRUN_EXPR
			_isExtFut = true;
#else
			if (newStandPat - Searcher::getFutMgn(getDepth(), getMoveCount()) >= -alpha) {
				pruning = true;
			}
#endif
		}
	}

	bool Searcher::NodeController::connectedThreat(
			Tree& tree, const Move& threat, const Move& move) {
		if (threat.isEmpty() || move.isEmpty()) {
			return false;
		}
		assert(threat.getPiece().isBlack() != move.getPiece().isBlack());

		if (threat.getTo() == move.getFrom()) {
			return true;
		}

		if (threat.getTo() == move.getTo()) {
			return true;
		}

		// TODO

		return false;
	}
}
