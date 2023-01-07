/*
 * timeTest.h
 *
 *  Created on: 2013/04/28
 *      Author: ryosuke
 */

#ifndef TIMETEST_H_
#define TIMETEST_H_

#include "test.h"

namespace Tests {
	class TimeTest : public Test {
	private:
		TestResult timeMonotonicTest();

	protected:
		void execute() {
			addResult("timeMonotonicTest", timeMonotonicTest());
		}

		const char* name() {
			return "EVAL_TEST";
		}

	public:
	};
}

#endif // TIMETEST_H_
