/*
 * board.cpp
 *
 *  Created on: 2012/05/10
 *      Author: ryosuke
 */

#include <sstream>
#include <iostream>
#include <cstring>
#include "board.h"
#include "../Csa/csa.h"

namespace Shogi {
	const Piece Board::even[Square::RANK_NUM][Square::FILE_NUM] = {
			{ Piece::WLANCE, Piece::WKNIGHT, Piece::WSILVER, Piece::WGOLD, Piece::WKING, Piece::WGOLD, Piece::WSILVER, Piece::WKNIGHT, Piece::WLANCE, },
			{ Piece::EMPTY , Piece::WBISHOP, Piece::EMPTY  , Piece::EMPTY, Piece::EMPTY, Piece::EMPTY, Piece::EMPTY  , Piece::WROOK  , Piece::EMPTY, },
			{ Piece::WPAWN , Piece::WPAWN  , Piece::WPAWN  , Piece::WPAWN, Piece::WPAWN, Piece::WPAWN, Piece::WPAWN  , Piece::WPAWN  , Piece::WPAWN, },
			{ Piece::EMPTY , Piece::EMPTY  , Piece::EMPTY  , Piece::EMPTY, Piece::EMPTY, Piece::EMPTY, Piece::EMPTY  , Piece::EMPTY  , Piece::EMPTY, },
			{ Piece::EMPTY , Piece::EMPTY  , Piece::EMPTY  , Piece::EMPTY, Piece::EMPTY, Piece::EMPTY, Piece::EMPTY  , Piece::EMPTY  , Piece::EMPTY, },
			{ Piece::EMPTY , Piece::EMPTY  , Piece::EMPTY  , Piece::EMPTY, Piece::EMPTY, Piece::EMPTY, Piece::EMPTY  , Piece::EMPTY  , Piece::EMPTY, },
			{ Piece::BPAWN , Piece::BPAWN  , Piece::BPAWN  , Piece::BPAWN, Piece::BPAWN, Piece::BPAWN, Piece::BPAWN  , Piece::BPAWN  , Piece::BPAWN, },
			{ Piece::EMPTY , Piece::BROOK  , Piece::EMPTY  , Piece::EMPTY, Piece::EMPTY, Piece::EMPTY, Piece::EMPTY  , Piece::BBISHOP, Piece::EMPTY, },
			{ Piece::BLANCE, Piece::BKNIGHT, Piece::BSILVER, Piece::BGOLD, Piece::BKING, Piece::BGOLD, Piece::BSILVER, Piece::BKNIGHT, Piece::BLANCE, },
	};

	void Board::init(Handicap handicap) {
		for (Square sq(Square::TOP_W); sq.valid(); sq.inc()) {
			set(sq, Piece::WALL);
		}

		if (handicap == NOPIECES) {
			for (Square sq(Square::TOP); sq.valid(); sq.next()) {
				set(sq, Piece::EMPTY);
			}
		} else {
			for (Square sq(Square::TOP); sq.valid(); sq.next()) {
				set(sq, even[sq.getRank()-1][sq.getFile()-1]);
			}
			switch (handicap) {
			case HANDICAP_8PIECES:
				set(Square(3, 1), Piece::EMPTY);
				set(Square(7, 1), Piece::EMPTY);
			case HANDICAP_6PIECES:
				set(Square(2, 1), Piece::EMPTY);
				set(Square(8, 1), Piece::EMPTY);
			case HANDICAP_4PIECES:
				set(Square(1, 1), Piece::EMPTY);
				set(Square(9, 1), Piece::EMPTY);
			case HANDICAP_2PIECES:
				set(Square(2, 2), Piece::EMPTY);
				set(Square(8, 2), Piece::EMPTY);
				break;
			case HANDICAP_ROOK_LANCE:
				set(Square(1, 1), Piece::EMPTY);
			case HANDICAO_ROOK:
				set(Square(8, 2), Piece::EMPTY);
				break;
			case HANDICAP_BISHOP:
				set(Square(2, 2), Piece::EMPTY);
				break;
			case HANDICAP_LANCE:
				set(Square(1, 1), Piece::EMPTY);
				break;
			default:
				break;
			}
		}
	}

	void Board::init(const Board& b) {
#if 0
		for (Square pos(Square::TOP_W); pos.valid(); pos.inc()) {
			set(pos, b.get(pos));
		}
#else
		memcpy(&board, &b.board, sizeof(board));
#endif
	}

	template <bool black>
	Square Board::getKingSquare() const {
		for(Square sq = Square::TOP; sq.valid(); sq.next()) {
			if (get(sq) == (black ? Piece::BKING : Piece::WKING)) {
				return sq;
			}
		}
		return Square::INVALID;
	}
	template Square Board::getKingSquare<true>() const;
	template Square Board::getKingSquare<false>() const;

	template <bool black>
	PawnFlags Board::getPawnFiles() const {
		PawnFlags flags;
		for(Square sq = Square::TOP; sq.valid(); sq.next()) {
			if (get(sq) == (black ? Piece::BPAWN : Piece::WPAWN)) {
				flags.set(sq.getFile());
			}
		}
		return flags;
	}
	template PawnFlags Board::getPawnFiles<true>() const;
	template PawnFlags Board::getPawnFiles<false>() const;

	std::string Board::toString() const {
		std::ostringstream oss;
		Square sq(Square::TOP);
		for (sq.leftmost(); !get(sq).isWall(); sq.leftmost(), sq.down()) {
			for (; !get(sq).isWall(); sq.right()) {
				oss << get(sq).toString();
			}
			oss << '\n';
		}
		return oss.str();
	}

	std::string Board::toStringCsa() const {
		std::ostringstream oss;
		Square sq(Square::TOP);
		for (sq.leftmost(); !get(sq).isWall(); sq.leftmost(), sq.down()) {
			oss << Csa::CHAR_POS << sq.getRank();
			for (; !get(sq).isWall(); sq.right()) {
				oss << get(sq).toStringCsa();
			}
			oss << '\n';
		}
		return oss.str();
	}
}
