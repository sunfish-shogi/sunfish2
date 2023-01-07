/*
 * phasedMoveGenerator.h
 *
 *  Created on: 2012/07/06
 *      Author: ryosuke
 */

#ifndef PHASED_MOVE_GENERATOR_H_
#define PHASED_MOVE_GENERATOR_H_

#include "../Shogi/moveGenerator.h"
#include "../Evaluates/param.h"
#include "killer.h"
#include "hashMove.h"
#include "history.h"

namespace Search {
	enum PHASE {
		PHASE_BEGIN = 0, // hash
		PHASE_END,

		PHASE_CAPTURE,
		PHASE_NOCAPTURE,

		PHASE_TACTICAL,

		PHASE_CAPTURE_ONLY,

		PHASE_CHECK_ONLY,
	};

	class PhasedMoveGenerator : public Shogi::MoveGenerator {
	private:
		PHASE phase;
		HashMove hashMove;
		const Evaluates::Param& param;
		const History& history;
		unsigned hashNum;
		Killer killer;

		void removeKillerMove(int begin, int num);

		void removeHashMove(int begin, int num);

		template <bool plusOnly>
		void sortSee(int begin, int num);

		void sortHistory(int begin, int num);

	public:
		typedef Shogi::MoveGenerator Super;

		PhasedMoveGenerator(const Shogi::Position& pos,
				const Evaluates::Param& param,
				const History& history) :
				Shogi::MoveGenerator(pos),
				param(param), history(history) {
			init();
		}

		void clear() {
			Shogi::MoveGenerator::clear();
			hashMove.init();
		}

		void init(PHASE firstPhase = PHASE_BEGIN) {
			Shogi::MoveGenerator::clear();
			phase = firstPhase;
			hashNum = 0;
		}

		void initTac() {
			init(PHASE_TACTICAL);
		}

		void initCap() {
			init(PHASE_CAPTURE_ONLY);
		}

		void initChk() {
			init(PHASE_CHECK_ONLY);
		}

		void generateAll();

		const Shogi::Move* next();

		void setHashMove(const HashMove& hashMove) {
			this->hashMove = hashMove;
		}

		bool isHashMove() const {
			const Shogi::Move* pmove = get();
			assert(pmove != NULL);
			return getCurrent() <= hashNum ||
					*pmove == killer.get1() ||
					*pmove == killer.get2();
		}

		void sort(Evaluates::Value values[]);

		Killer& getKiller() {
			return killer;
		}
	};
}

#endif // PHASED_MOVE_GENERATOR_H_
