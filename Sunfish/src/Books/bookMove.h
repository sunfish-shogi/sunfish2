/*
 * bookMove.h
 *
 *  Created on: 2013/03/10
 *      Author: ryosuke
 */

#ifndef BOOKMOVE_H_
#define BOOKMOVE_H_

#include "../Shogi/move.h"

namespace Books {
	class BookMove {
	private:
		Shogi::Move move;
		unsigned count;

	public:
		BookMove(const Shogi::Move& move) {
			this->move = move;
			count = 1;
		}

		BookMove(const Shogi::Move& move, unsigned count) {
			this->move = move;
			this->count = count;
		}

		BookMove(const BookMove& bookMove) {
			move = bookMove.move;
			count = bookMove.count;
		}

		unsigned addCount() {
			return ++count;
		}

		void setCount(unsigned count) {
			this->count = count;
		}

		unsigned getCount() const {
			return count;
		}

		bool is(const Shogi::Move& move) const {
			return this->move == move;
		}

		const Shogi::Move& getMove() const {
			return move;
		}
	};
}

#endif // BOOKMOVE_H_
