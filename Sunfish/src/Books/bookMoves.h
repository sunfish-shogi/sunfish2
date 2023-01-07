/*
 * bookMoves.h
 *
 *  Created on: 2013/03/10
 *      Author: ryosuke
 */

#ifndef BOOKMOVES_H_
#define BOOKMOVES_H_

#include "bookMove.h"
#include "../Util/int.h"
#include "../Util/random.h"
#include "../Log/logger.h"
#include <vector>

namespace Books {
	class BookMovesBase {
	private:
		Util::uint64 hash;
		unsigned count;
		unsigned threshold;

	public:
		BookMovesBase(Util::uint64 hash, unsigned threshold) :
				hash(hash), count(0), threshold(threshold) {
		}

		BookMovesBase(Util::uint64 hash, unsigned count,
				unsigned threshold) : hash(hash),
				count(count), threshold(threshold) {
		}

		Util::uint64 getHash() const {
			return hash;
		}

		unsigned getCount() const {
			return count;
		}

		unsigned getThreshold() const {
			return threshold;
		}
	};

	class BookMoves {
	private:
		static const int NOT_EXISTS = -1;

		Util::uint64 hash;
		std::vector<BookMove> moves;
		unsigned count;
		// 何手以上あれば有効とみなすかのしきい値
		unsigned threshold;

		int getIndex(const Shogi::Move& move) const {
			for (unsigned i = 0; i < moves.size(); i++) {
				if (moves[i].is(move)) {
					return i;
				}
			}
			return NOT_EXISTS;
		}

	public:
		BookMoves(const BookMovesBase& base) :
				hash(base.getHash()),
				count(base.getCount()),
				threshold(base.getThreshold()) {
		}

		Util::uint64 getHash() const {
			return hash;
		}

		int addMove(const Shogi::Move& move) {
			int index = getIndex(move);
			if (index == NOT_EXISTS) {
				moves.push_back(BookMove(move));
				return 1;
			} else {
				unsigned c = moves[index].addCount();
				if (c == threshold) {
					count += c;
				} else if (c > threshold) {
					count++;
				}
				return c;
			}
		}

		void setMove(const Shogi::Move& move, unsigned count,
				bool overwrite = true) {
			if (count >= threshold) {
				this->count += count;
			}
			int index;
			if (overwrite &&
					(index = getIndex(move)) != NOT_EXISTS) {
				moves[index].setCount(count);
			} else {
				moves.push_back(BookMove(move, count));
			}
		}

		unsigned getCount() const {
			return count;
		}

		unsigned getSize() const {
			return moves.size();
		}

		unsigned getCount(unsigned index) const {
			if (index >= moves.size()) {
				return 0;
			}
			return moves[index].getCount();
		}

		const Shogi::Move* getMove(unsigned index) const {
			if (index >= moves.size()) {
				return NULL;
			}
			return &moves[index].getMove();
		}

		const Shogi::Move* getMove(Util::Random& random) const {
			if (count == 0) {
				return NULL;
			}
			unsigned r = random.getInt32(count);
			unsigned i = 0;
			for (; i < moves.size(); i++) {
				unsigned c = moves[i].getCount();
				if (c < threshold) {
					continue;
				}
				if (r < c) {
					return &moves[i].getMove();
				}
				r -= c;
			}
			return NULL;
		}
	};
}

#endif // BOOKMOVES_H_
