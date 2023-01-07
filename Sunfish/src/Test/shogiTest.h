/*
 * shogiTest.h
 *
 *  Created on: 2013/02/02
 *      Author: ryosuke
 */

#ifndef SHOGITEST_H_
#define SHOGITEST_H_

#include "test.h"

namespace Tests {
	class ShogiTest : public Test {
	private:

		TestResult generatorRandomTest(const char* filename = NULL);

		TestResult generatorTest(const char* filename = NULL);

	protected:
		void execute() {
			addResult("generatorRandomTest", generatorRandomTest());
			//addResult("generatorTest", generatorTest());
		}

		const char* name() {
			return "SHOGI_TEST";
		}
	};
}

#endif // SHOGITEST_H_
