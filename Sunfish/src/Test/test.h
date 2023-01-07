/*
 * test.h
 *
 *  Created on: 2013/02/02
 *      Author: ryosuke
 */

#ifndef TEST_H_
#define TEST_H_

#include <sstream>
#include <string>
#include "../Log/logger.h"

#define TEST_RESULT(ok)			TestResult(__FILE__, __LINE__, (ok))

namespace Tests {
	class Test {
	private:
		int success;
		int fail;

	protected:
		class TestResult {
		private:
			const char* file;
			unsigned line;
			bool ok;
		public:
			TestResult(const TestResult& info) :
				file(info.file), line(info.line), ok(info.ok) {
			}
			TestResult(const char* file, unsigned line,
				bool ok) : file(file), line(line), ok(ok) {
			}
			bool isOk() const {
				return ok;
			}
			std::string toString() const {
				std::ostringstream oss;
				oss << file << '(' << line << ")\n";
				return oss.str();
			}
		};

		bool addResult(const char* name, const TestResult& info) {
			if (info.isOk()) {
				Log::test << "SUCCESS:" << name << ' ' << info.toString() << '\n';
				success++;
				return true;
			} else {
				Log::test << "FAIL:" << name << ' ' << info.toString() << '\n';
				fail++;
				return false;
			}
		}

		virtual void execute() = 0;

		virtual const char* name() = 0;

	public:
		bool test() {
			success = fail = 0;
			Log::test << "=============== " << name() << " ===============" << '\n';
			execute();
			Log::test << "success:" << success << ' ';
			Log::test << "fail:" << fail << '\n';
			if (success + fail == 0) {
				Log::test << "no tests are executed!\n";
			} else if (fail == 0) {
				Log::test << "all tests succeeded.\n";
			} else {
				Log::test << "some tests failed!!\n";
			}
			return fail == 0;
		}
	};
}

#endif // TEST_H_
