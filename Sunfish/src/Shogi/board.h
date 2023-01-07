/*
 * board.h
 *
 *  Created on: 2012/05/10
 *      Author: ryosuke
 */

#ifndef BOARD_H_
#define BOARD_H_

#include "piece.h"
#include "handicap.h"
#include "square.h"
#include "pawnFlags.h"
#include <string>
#include <iostream>

namespace Shogi {
	class Board {
	private:
		static const Piece even[Square::RANK_NUM][Square::FILE_NUM];
		Piece board[Square::SIZE];

	public:
		Board(Handicap handicap = EVEN) {
			init(handicap);
		}

		Board(const Board& board) {
			init(board);
		}

		void init(Handicap handicap = EVEN);

		void init(const Board& b);

		const Piece& get(const Square& sq) const {
			return board[sq.getIndex()];
		}

		Piece set(const Square& sq, const Piece& piece) {
			Piece oldPiece = board[sq.getIndex()];
			board[sq.getIndex()] = piece;
			return oldPiece;
		}

		template <bool black>
		Square getKingSquare() const;

		template <bool black>
		PawnFlags getPawnFiles() const;

		std::string toString() const;

		std::string toStringCsa() const;

		bool writeBinary(std::ostream& out) const {
			for (Square sq = Square::TOP; sq.valid(); sq.next()) {
				if (!board[sq].writeBinary(out)) {
					return false;
				}
			}
			return true;
		}

		bool readBinary(std::istream& in) {
			for (Square sq = Square::TOP; sq.valid(); sq.next()) {
				if (!board[sq].readBinary(in)) {
					return false;
				}
			}
			return true;
		}
	};
}

#endif /* BOARD_H_ */
