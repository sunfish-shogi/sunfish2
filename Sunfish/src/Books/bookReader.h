/*
 * bookReader.h
 *
 *  Created on: 2013/03/14
 *      Author: ryosuke
 */

#ifndef BOOKREADER_H_
#define BOOKREADER_H_

#include "book.h"
#include <iostream>

namespace Books {
	class BookReader {
	private:
		BookReader() {
		}

		static bool read(std::istream& in, Book& book);

		static bool read(std::istream& in, Book& book, Util::uint64 hash);

	public:
		static bool read(const char* filename, Book& book);
	};
}

#endif // BOOKREADER_H_
