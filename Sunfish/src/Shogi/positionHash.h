/*
 * positionHash.h
 *
 *  Created on: 2012/06/10
 *      Author: ryosuke
 */

#ifndef POSITIONHASH_H_
#define POSITIONHASH_H_

#include "../Util/hash.h"
#include "piece.h"
#include "square.h"
#include "hand.h"

namespace Shogi {
	class PositionHash {
	private:
		enum {
			SIZE_BOARD = (Piece::WDRAGON+1) * (Square::END+1),
			SIZE_BLACK_HAND = (Piece::WDRAGON+1) * (Hand::MAX),
			SIZE_WHITE_HAND = (Piece::WDRAGON+1) * (Hand::MAX),
			SIZE_BLACK = 1,

			SIZE_ALL = SIZE_BOARD + SIZE_BLACK_HAND
					+ SIZE_WHITE_HAND + SIZE_BLACK,
		};

		Util::uint64 (*board)[Square::END+1];
		Util::uint64 (*blackHand)[Hand::MAX];
		Util::uint64 (*whiteHand)[Hand::MAX];
		Util::uint64 black;

	public:
		PositionHash(Util::Hash& hash) {
			init(hash);
		}

		void init(Util::Hash& hash) {
			Util::uint64* values;
			values = hash.get(SIZE_BOARD);
			board = (Util::uint64 (*)[Square::END+1])values;
			values = hash.get(SIZE_BLACK_HAND);
			blackHand = (Util::uint64 (*)[Hand::MAX])values;
			values = hash.get(SIZE_WHITE_HAND);
			whiteHand = (Util::uint64 (*)[Hand::MAX])values;
			values = hash.get(SIZE_BLACK);
			black = values[0];
		}

		Util::uint64 getBoard(const Piece& piece, const Square& square) const {
			return board[piece][square];
		}

		Util::uint64 getBlackHand(const Piece& piece, int h) const {
			return blackHand[piece.getTurnedBlack()][h];
		}

		Util::uint64 getWhiteHand(const Piece& piece, int h) const {
			return whiteHand[piece.getTurnedBlack()][h];
		}

		Util::uint64 getBlack() const {
			return black;
		}

		static unsigned getRequireSize() {
			return SIZE_ALL;
		}
	};
}

#endif // POSITIONHASH_H_
