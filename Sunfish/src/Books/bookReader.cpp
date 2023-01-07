/*
 * bookReader.cpp
 *
 *  Created on: 2013/03/14
 *      Author: ryosuke
 */

#include "bookReader.h"
#include "../Log/logger.h"
#include <fstream>

namespace Books {
	using namespace Shogi;

	bool BookReader::read(const char* filename, Book& book) {
		Log::message.fileOpenMessage(filename);
		std::ifstream fin(filename, std::ios::in | std::ios::binary);
		if (!fin) {
			Log::warning.fileOpenError(filename);
			return false;
		}
		bool ok = read(fin, book);
		fin.close();
		return ok;
	}

	bool BookReader::read(std::istream& in, Book& book) {
		while (true) {
			Util::uint64 hash;
			in.read((char*)&hash, sizeof(hash));
			if (in.eof()) { break; }
			if (!in) {
				Log::error.ioError();
			}
			if (!read(in, book, hash)) {
				return false;
			}
		}
		return true;
	}

	bool BookReader::read(std::istream& in, Book& book, Util::uint64 hash) {
		BookMoves& bookMoves = book.putMoves(hash, false);
		while (true) {
			unsigned m, count;
			in.read((char*)&m, sizeof(m));
			if (in && m == 0) { break; }
			in.read((char*)&count, sizeof(count));
			if (in.eof()) {
				Log::error.formatError();
				return false;
			}
			if (!in) {
				Log::error.ioError();
			}
			Move move(m);
			bookMoves.setMove(move, count, false);
		}
		return true;
	}
}
