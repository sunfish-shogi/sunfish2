/*
 * evalTest.cpp
 *
 *  Created on: 2013/04/27
 *      Author: ryosuke
 */

#ifndef NDEBUG

#include <boost/random.hpp>
#include <boost/thread.hpp>
#include <ctime>
#include "evalTest.h"
#include "../Evaluates/evaluate.h"
#include "../Csa/csaReader.h"
#include "../Shogi/moveGenerator.h"
#include "../Util/random.h"

namespace Tests {
	using namespace Shogi;
	using namespace Evaluates;

	Test::TestResult EvalTest::evaluateRandomTest(const char* filename) {
		Evaluate eval(*pparam);
		Position pos(EVEN);
		if (filename) {
			Csa::CsaReader::read(filename, pos);
		}

		Util::Random r;
		for (int i = 0; i < 10000; i++) {
			MoveGenerator gen(pos);
			gen.generate();

			std::string prevPos = pos.toString();

			if (gen.getNumber() == 0) {
				break;
			}
			const Move& move = gen.get(r.getInt32(gen.getNumber()));
			pos.moveUnsafe(move, eval);

			Evaluate curr(*pparam, pos);
			if (eval.getValue() != curr.getValue()) {
				Log::test << prevPos;
				Log::test << move.toString() << '\n';
				Log::test << "diff:" << eval.getValue() << '\n';
				Log::test << "stat:" << curr.getValue() << '\n';
				Log::test << "diff(base):" << eval.getBaseValue() << '\n';
				Log::test << "stat(base):" << curr.getBaseValue() << '\n';
				Log::test << "diff(pos):" << eval.getPositionalValue() << '\n';
				Log::test << "stat(pos):" << curr.getPositionalValue() << '\n';
				return TEST_RESULT(false);
			}
		}
		return TEST_RESULT(true);
	}
}

#endif

