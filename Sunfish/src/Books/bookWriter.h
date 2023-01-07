/*
 * bookWriter.h
 *
 *  Created on: 2013/03/10
 *      Author: ryosuke
 */

#ifndef BOOKWRITER_H_
#define BOOKWRITER_H_

#include "book.h"
#include <iostream>

namespace Books {
	class BookWriter {
	private:
		BookWriter() {
		}

		static bool write(std::ostream& out, const BookEntity& entity);

		static bool write(std::ostream& out, const BookMoves& moves);

	public:
		static bool write(const char* filename, const Book& book);
	};
}

#endif // BOOKWRITER_H_
