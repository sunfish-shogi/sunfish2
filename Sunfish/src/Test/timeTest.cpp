/*
 * timeTest.cpp
 *
 *  Created on: 2013/04/28
 *      Author: ryosuke
 */

#ifndef NDEBUG

#include "timeTest.h"
#include "../Util/timer.h"
#include "../Log/logger.h"

namespace Tests {
	Test::TestResult TimeTest::timeMonotonicTest() {
		const int intervalNanos = 50 * 1000 * 1000;
		struct timespec interval  = { 0, intervalNanos };
		const int limitSec = 10 * 60;
		Util::Timer timer;
		timer.set();
		double prev = timer.get();
		while (prev <= limitSec) {
			nanosleep(&interval, NULL);
			double curr = timer.get();
			if (curr < prev + 1.0e-9 * intervalNanos) {
				Log::test << "prev=" << prev << '\n';
				Log::test << "curr=" << curr << '\n';
				return TEST_RESULT(false);
			}
			prev = curr;
		}
		return TEST_RESULT(true);
	}
}

#endif
