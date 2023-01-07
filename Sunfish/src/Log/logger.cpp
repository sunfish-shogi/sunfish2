/*
 * log.cpp
 * 
 *  Created on: 2012/07/29
 *      Author: Kubo Ryosuke
 */

#include "logger.h"

namespace Log {
	Logger error("ERROR");
	Logger warning("WARNING");
	Logger message;
	Logger send("SEND");
	Logger receive("RECEIVE");
#ifndef NDEBUG
	Logger debug("DEBUG");
	Logger test("TEST");
#endif //NDEBUG
#ifdef PRUN_EXPR
	Logger expr;
#endif //PRUN_EXPR

	void Logger::putPrefix() {
		if (name != NULL) {
			*this << name << ": ";
		}
	}

	void Logger::unknownError(const char* detail) {
		putPrefix();
		*this << "unknown error.";
		if (detail != NULL) {
			*this << " (" << detail << ')';
		}
		*this << '\n';
	}

	void Logger::fileOpenMessage(const char* filename) {
		putPrefix();
		*this << "open a file. (file=[" << filename << "])\n";
	}

	void Logger::fileOpenError(const char* filename) {
		putPrefix();
		*this << "open error. (file=[" << filename << "])\n";
	}

	void Logger::fileIoError(const char* filename,
			const char* detail) {
		putPrefix();
		*this << "file I/O error. (file=[" << filename << "]";
		if (detail != NULL) {
			*this << ", " << detail;
		}
		*this << ")\n";
	}

	void Logger::ioError(const char* detail) {
		putPrefix();
		*this << "I/O error.";
		if (detail != NULL) {
			*this << " (" << detail << ')';
		}
		*this << '\n';
	}

	void Logger::fileFormatError(const char* filename,
			const char* str, int column) {
		putPrefix();
		*this << "file format error. (file=[" << filename << "])\n";
		_formatError(str, column);
	}

	void Logger::fileFormatError(const char* filename,
			int line, const char* str, int column) {
		putPrefix();
		*this << "file format error at line " << line
				<< ". (file=[" << filename << "])\n";
		_formatError(str, column);
	}

	void Logger::formatError(const char* str, int column) {
		putPrefix();
		*this << "format error.\n";
		_formatError(str, column);
	}

	void Logger::formatError(int line, const char* str, int column) {
		putPrefix();
		*this << "format error at line " << line << ".\n";
		_formatError(str, column);
	}

	void Logger::_formatError(const char* str, int column) {
		if (str != NULL) {
			*this << str << '\n';
			if (column >= 0) {
				for (int i = 0; i < column; i++) {
					*this << ' ';
				}
				*this << "^\n";
			}
		}
	}
}
