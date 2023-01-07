/*
 * bookWriter.cpp
 *
 *  Created on: 2013/03/10
 *      Author: ryosuke
 */

#include "bookWriter.h"
#include "../Log/logger.h"
#include <fstream>

namespace Books {
	using namespace Shogi;

	bool BookWriter::write(const char* filename, const Book& book) {
		Log::message.fileOpenMessage(filename);
		std::ofstream fout(filename, std::ios::out | std::ios::binary);
		if (!fout) {
			Log::warning.fileOpenError(filename);
			return false;
		}
		bool ok = true;
		for (unsigned index = 0; index < book.getSize(); index++) {
			const BookEntity& entity = book.getEntity(index);
			if (!write(fout, entity)) {
				ok = false;
				break;
			}
		}
		fout.close();
		return ok;
	}

	bool BookWriter::write(std::ostream& out, const BookEntity& entity) {
		const std::list<BookMoves>& chain = entity.getChain();
		std::list<BookMoves>::const_iterator it = chain.begin();
		for (; it != chain.end(); it++) {
			if (!write(out, *it)) {
				return false;
			}
		}
		return true;
	}

	bool BookWriter::write(std::ostream& out, const BookMoves& moves) {
		if (moves.getSize() > 0) {
			Util::uint64 hash = moves.getHash();
			out.write((char*)&hash, sizeof(hash));
			for (unsigned index = 0; index < moves.getSize(); index++) {
				unsigned move = (unsigned)*moves.getMove(index);
				unsigned count = moves.getCount(index);
				out.write((char*)&move, sizeof(move));
				out.write((char*)&count, sizeof(count));
			}
			unsigned empty = 0;
			out.write((char*)&empty, sizeof(empty));
		}
		return true;
	}
}
