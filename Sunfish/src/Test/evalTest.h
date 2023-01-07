/*
 * evalTest.h
 *
 *  Created on: 2013/04/27
 *      Author: ryosuke
 */

#ifndef EVALTEST_H_
#define EVALTEST_H_

#include "test.h"
#include "../Evaluates/initializer.h"

namespace Tests {
	class EvalTest : public Test {
	private:
		Evaluates::Param* pparam;

		TestResult evaluateRandomTest(const char* filename = NULL);

	protected:
		void execute() {
			addResult("evaluateRandomTest", evaluateRandomTest());
		}

		const char* name() {
			return "EVAL_TEST";
		}

	public:
		EvalTest() {
			pparam = new Evaluates::Param();
			Evaluates::Initializer::apply(*pparam);
			pparam->read("evdata");
		}
		
		~EvalTest() {
			delete pparam;
		}
	};
}

#endif // EVALTEST_H_
