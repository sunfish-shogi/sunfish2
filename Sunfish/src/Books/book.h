/*
 * book.h
 *
 *  Created on: 2013/03/10
 *      Author: ryosuke
 */

#ifndef BOOK_H_
#define BOOK_H_

#include "bookEntity.h"
#include "../Table/baseTable.h"

namespace Books {
	class Book : public Table::BaseTable<BookEntity> {
	private:
		unsigned threshold;
		Util::Random random;

	public:
		Book(unsigned bits, unsigned threshold) :
				Table::BaseTable<BookEntity>(bits),
				threshold(threshold) {
		}

		~Book() {
		}

		const Shogi::Move* getMove(Util::uint64 hash) {
			return getEntity(hash).getMove(hash, random);
		}

		const BookMoves* getMoves(Util::uint64 hash) const {
			return getEntity(hash).getMoves(hash);
		}

		unsigned addMove(Util::uint64 hash,
				const Shogi::Move& move) {
			return _getEntity(hash)
					.addMove(hash, move, threshold);
		}

		void setMove(Util::uint64 hash, const Shogi::Move& move,
				unsigned count, bool overwrite = true) {
			_getEntity(hash).setMove(hash, move,
					count, threshold, overwrite);
		}

		BookMoves& putMoves(Util::uint64 hash,
				bool overwrite = true) {
			return _getEntity(hash)
					.putMoves(hash, threshold, overwrite);
		}
	};
}

#endif // BOOK_H_
