/*
 * log.h
 * 
 *  Created on: 2012/07/29
 *      Author: Kubo Ryosuke
 */

#ifndef LOGGER_H_
#define LOGGER_H_

#include <iostream>
#include <vector>
#include <ctime>

#define __THIS__			__FILE__ << '(' << __LINE__ << ')'

namespace Log {
	class Logger {
	public:
		class SubLogger {
		private:
			Logger& logger;

		public:
			SubLogger(Logger& logger) : logger(logger) {
			}

			template <class T>
			SubLogger& operator<<(T t) {
				logger.print(t);
				return *this;
			}
		};

	private:
		struct STREAM {
			std::ostream* pout;
			const char* before;
			const char* after;
			bool time;
		};

		SubLogger sub;

		const char* name;

		std::vector<STREAM> os;

		void putPrefix();

		void _formatError(const char* str = NULL, int column = -1);

	public:
		Logger(const char* name = NULL) :
				sub(*this), name(name) {
		}

		void addStream(std::ostream& o, bool time) {
			addStream(o, NULL, NULL, time);
		}

		void addStream(std::ostream& o) {
			addStream(o, NULL, NULL);
		}

		void addStream(std::ostream& o,
				const char* before,
				const char* after,
				bool time = false) {
			STREAM s = { &o, before, after, time };
			os.push_back(s);
		}

		template <class T> void print(const T t, bool top = false) {
			std::vector<STREAM>::iterator it;
			for (it = os.begin(); it != os.end(); it++) {
				if (it->before != NULL) {
					*(it->pout) << it->before;
				}
				if (top && it->time) {
					time_t t = time(NULL);
					struct tm lt;
					localtime_r(&t, &lt);
					char tstr[32];
					strftime(tstr, sizeof(tstr)-1, "%Y-%m-%dT%H:%M:%S\t", &lt);
					*(it->pout) << tstr;
				}
				*(it->pout) << t;
				if (it->after != NULL) {
					*(it->pout) << it->after;
				}
				it->pout->flush();
			}
		}

		template <class T>
		SubLogger& operator<<(const T t) {
			print(t, true);
			return sub;
		}

		void unknownError(const char* detail = NULL);

		void fileOpenMessage(const char* filename);

		void fileOpenError(const char* filename);

		void fileIoError(const char* filename,
				const char* detail = NULL);

		void ioError(const char* detail = NULL);

		void fileFormatError(const char* filename,
				const char* str = NULL, int column = -1);

		void fileFormatError(const char* filename, int line,
				const char* str = NULL, int column = -1);

		void formatError(const char* str = NULL, int column = -1);

		void formatError(int line, const char* str = NULL, int column = -1);
	};

	extern Logger error;
	extern Logger warning;
	extern Logger message;
	extern Logger send;
	extern Logger receive;
#ifndef NDEBUG
	extern Logger debug;
	extern Logger test;
#endif //NDEBUG
#ifdef PRUN_EXPR
	extern Logger expr;
#endif //PRUN_EXPR
}

#endif // LOGGER_H_
