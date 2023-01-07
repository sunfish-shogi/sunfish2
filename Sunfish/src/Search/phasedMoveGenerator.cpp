/*
 * phasedMoveGenerator.cpp
 *
 *  Created on: 2012/07/06
 *      Author: ryosuke
 */

#include "phasedMoveGenerator.h"
#include "attackers.h"

namespace Search {
	using namespace Shogi;
	using namespace Evaluates;

	void PhasedMoveGenerator::sort(Value values[]) {
		for (unsigned i = 0; i < getNumber(); i++) {
			unsigned j;
			Value value = values[i];
			for (j = i; j > 0; j--) {
				if (values[j-1] >= value) {
					break;
				}
				values[j] = values[j-1];
			}
			values[j] = value;
			insertBefore(i, j);
		}
	}

	template <bool plusOnly>
	void PhasedMoveGenerator::sortSee(int begin, int num) {
		Value values[num];
		for (int i = 0; i < num; i++) {
			const Move& move = get(begin + i);
			Attackers attackers(param, getPosition(), move);
			Value value = attackers.see();
			if (killer.get1() == move) {
				Value kvalue = killer.getValue1() +
						Killer::calcCurrentChange(getPosition(), move, param);
				value = Value::max(value, kvalue);
			} else if (killer.get2() == move) {
				Value kvalue = killer.getValue2() +
						Killer::calcCurrentChange(getPosition(), move, param);
				value = Value::max(value, kvalue);
			}
			int j;
			for (j = i; j > 0; j--) {
				if (values[j-1] >= value) {
					break;
				}
				values[j] = values[j-1];
			}
			values[j] = value;
			insertBefore(begin + i, begin + j);
		}
		if (plusOnly) {
			// see が正の値のものに限定
			for (int i = 0; i < num; i++) {
				if (values[i] <= 0) {
					removeAfter(i);
					break;
				}
			}
		}
	}

	void PhasedMoveGenerator::sortHistory(int begin, int num) {
		Value values[num];
		for (int i = 0; i < num; i++) {
			Value value = history.get(get(begin+i));
			int j;
			for (j = i; j > 0; j--) {
				if (values[j-1] >= value) {
					break;
				}
				values[j] = values[j-1];
			}
			values[j] = value;
			insertBefore(begin + i, begin + j);
		}
	}

	void PhasedMoveGenerator::removeKillerMove(int begin, int num) {
		for (int i = 0; i < num; i++) {
			const Move& move = get(begin + i);
			if (move == killer.get1() ||
					move == killer.get2()) {
				removeUnstable(begin + i--);
				num--;
			}
		}
	}

	void PhasedMoveGenerator::removeHashMove(int begin, int num) {
		for (int i = 0; i < num; i++) {
			const Move& move = get(begin + i);
			if (move == hashMove.getHash1() ||
					move == hashMove.getHash2()) {
				removeUnstable(begin + i--);
				num--;
			}
		}
	}

	// 非コルーチン
	void PhasedMoveGenerator::generateAll() {
		// 初期化
		Shogi::MoveGenerator::clear();
		// 駒を取る手
		generateCapture();
		sortSee<false>(0, getNumber());
		// 駒を取らない手
		int prevNum = getNumber();
		generateNocapture();
		sortHistory(prevNum, getNumber()-prevNum);
		// phase
		phase = PHASE_END;
	}

	const Move* PhasedMoveGenerator::next() {
		while (true) {
			const Move* pmove = Super::next();
			if (pmove != NULL) {
				return pmove;
			}
			int prevNum = getNumber();
			switch (phase) {
			case PHASE_BEGIN:
				if (!getPosition().isCheck()) {
					if (getPosition().isLegalMove(hashMove.getHash1(), true)) {
						add(hashMove.getHash1());
						hashNum++;
					}
					if (getPosition().isLegalMove(hashMove.getHash2(), true)) {
						add(hashMove.getHash2());
						hashNum++;
					}
				}
				phase = PHASE_CAPTURE;
				break;
			case PHASE_CAPTURE:
				generateCapture();
				if (!getPosition().isCheck()) {
					removeKillerMove(prevNum, getNumber()-prevNum);
					if (getPosition().isLegalMove(killer.get1(), true)) {
						add(killer.get1());
					}
					if (getPosition().isLegalMove(killer.get2(), true)) {
						add(killer.get2());
					}
					removeHashMove(prevNum, getNumber()-prevNum);
				}
				sortSee<false>(prevNum, getNumber()-prevNum);
				phase = PHASE_NOCAPTURE;
				break;
			case PHASE_NOCAPTURE:
				generateNocapture();
				if (!getPosition().isCheck()) {
					removeKillerMove(prevNum, getNumber()-prevNum);
					removeHashMove(prevNum, getNumber()-prevNum);
				}
				sortHistory(prevNum, getNumber()-prevNum);
				phase = PHASE_END;
				break;
			case PHASE_TACTICAL:
				generateTactical();
				sortSee<true>(prevNum, getNumber()-prevNum);
				phase = PHASE_END;
				break;
			case PHASE_CAPTURE_ONLY:
				generateCapture();
				sortSee<true>(prevNum, getNumber()-prevNum);
				phase = PHASE_END;
				break;
			case PHASE_CHECK_ONLY:
				generateCheck();
				// TODO: sort
				phase = PHASE_END;
				break;
			case PHASE_END:
				return NULL;
			}
		}
	}
}
