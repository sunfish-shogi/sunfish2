/*
 * shogiTest.cpp
 *
 *  Created on: 2013/02/02
 *      Author: ryosuke
 */

#ifndef NDEBUG

#include <boost/random.hpp>
#include <boost/thread.hpp>
#include <ctime>
#include "shogiTest.h"
#include "../Csa/csaReader.h"
#include "../Shogi/moveGenerator.h"
#include "../Tools/debug.h"

namespace Tests {
	using namespace Shogi;
	using namespace Tools;

	Test::TestResult ShogiTest::generatorRandomTest(const char* filename) {
		Position pos(EVEN);
		if (filename) {
			Csa::CsaReader::read(filename, pos);
		}
		boost::mt19937 rgen(static_cast<unsigned>(time(NULL)));
		for (int i = 0; i < 10000; i++) {
			MoveGenerator gen(pos);
			gen.generate();
			MoveGenerator gen2(pos);
			gen2.generateTardy();
	
			gen.sort();
			gen2.sort();
			if (!gen.equals(gen2)) {
				const Move* pmove;
				Log::test << pos.toStringCsa();
				Log::test << pos.toStringEffect(true);
				Log::test << Util::Int::toString64(pos.getHash()) << '\n';
				while ((pmove = gen.next()) != NULL) {
					Log::test << pmove->toString() << ' ';
				}
				Log::test << '\n';
				while ((pmove = gen2.next()) != NULL) {
					Log::test << pmove->toString() << ' ';
				}
				Log::test << '\n';
				return TEST_RESULT(false);
			}
	
			if (gen.getNumber() == 0) {
				//std::cout << "There is no a legal move.\n";
				return TEST_RESULT(true);
			}
	
			boost::uniform_smallint<> dst(0, gen.getNumber()-1);
			boost::variate_generator<boost::mt19937&, boost::uniform_smallint<> > r(rgen, dst);
			const Move& move = gen.get(r());
			pos.moveUnsafe(move);

			if (!Debug::positionOk(pos)) {
				Log::test << pos.toStringCsa();
				return TEST_RESULT(false);
			}
		}
		return TEST_RESULT(true);
	}

	// TODO: ディレクトリ内のすべてのファイルに対して行うように
	Test::TestResult ShogiTest::generatorTest(const char* filename) {
		Position pos(EVEN);
		if (filename) {
			Csa::CsaReader::read(filename, pos);
		}
		std::cout << pos.toStringCsa();
		std::cout << pos.toStringEffect();
		std::cout << pos.toStringBPawns() << '\n';
		std::cout << pos.toStringWPawns() << '\n';

		MoveGenerator gen(pos);
		gen.generate();
		gen.sort();
		MoveGenerator gen2(pos);
		gen2.generateTardy();
		gen2.sort();
		if (!gen.equals(gen2)) {
			const Move* pmove;
			while ((pmove = gen.next()) != NULL) {
				Log::test << pmove->toString() << ' ';
			}
			Log::test << '\n';
			while ((pmove = gen2.next()) != NULL) {
				Log::test << pmove->toString() << ' ';
			}
			Log::test << '\n';
		}
		return TEST_RESULT(true);
	}
}

#endif // NDEBUG
