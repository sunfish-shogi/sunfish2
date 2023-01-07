/*
 * effectBoard.cpp
 *
 *  Created on: 2012/05/15
 *      Author: ryosuke
 */

#include <iostream>
#include <sstream>
#include <cassert>
#include "../Util/bit.h"
#include "effectBoard.h"

namespace Shogi {
	void EffectBoard::init() {
		for (Square sq(Square::TOP_W); sq.valid(); sq.inc()) {
			set<true>(sq, DirectionFlags::WALL);
			set<false>(sq, DirectionFlags::WALL);
		}
		for (Square sq(Square::TOP); sq.valid(); sq.next()) {
			set<true>(sq, DirectionFlags::NON);
			set<false>(sq, DirectionFlags::NON);
		}
	}

	void EffectBoard::init(const Board& board) {
		init();
		for (Square sq(Square::TOP); sq.valid(); sq.next()) {
			if (board.get(sq).isBlack()) {
				createEffect<true>(board, sq);
			} else if (board.get(sq).isWhite()) {
				createEffect<false>(board, sq);
			}
		}
	}

	template <bool black>
	void EffectBoard::createEffect(const Board& board, const Square& sq) {
		const Piece& piece = board.get(sq);
		if (!piece.isEmpty()) {
			DirectionFlags dirFlags = piece.getMovableDirection();
			while (dirFlags.isNonZero()) {
				const DirectionFlags flag = dirFlags.pop();
				const Direction dir = flag.toDirection();
				for (Square to = sq + dir; !board.get(to).isWall(); to += dir) {
					effectBoard<black>()[to.getIndex()].add(flag);
					if (!board.get(to).isEmpty() || flag.shortRange()) {
						break;
					}
				}
			}
		}
	}

	template <bool black, bool addition>
	void EffectBoard::changeStraight(const Square& sq, const Direction dir, const DirectionFlags& flag, const Board& board) {
		for (Square to = sq + dir; !get(to, black).isWall(); to += dir) {
			if (addition) {
				effectBoard<black>()[to.getIndex()].add(flag);
			} else {
				effectBoard<black>()[to.getIndex()].remove(flag);
			}
			if (!board.get(to).isEmpty()) {
				break;
			}
		}
	}

	template <bool black, bool addition>
	void EffectBoard::changeAround(const Square& sq, const Board& board) {
		DirectionFlags flags = get(sq, black).getLongRangeAndKing();
		while (flags.isNonZero()) {
			DirectionFlags flag = flags.pop();
			Direction dir = flag.toDirection();
			changeStraight<black, addition>(sq, dir, flag, board);
		}
	}

	template <bool black, bool addition>
	void EffectBoard::change(const Square& sq, const DirectionFlags& dirFlags, const Board& board) {
		assert(sq.valid());
		assert(!board.get(sq).isWall());
		if (addition) {
			changeAround<true, false>(sq, board);
			changeAround<false, false>(sq, board);
		}
		DirectionFlags flags = dirFlags.getLongRangeAndKing();
		while (flags.isNonZero()) {
			DirectionFlags flag = flags.pop();
			Direction dir = flag.toDirection();
			changeStraight<black, addition>(sq, dir, flag, board);
		}
		flags = dirFlags.getShortRangeOnly();
		while (flags.isNonZero()) {
			DirectionFlags flag = flags.pop();
			Direction dir = flag.toDirection();
			Square to = sq + dir;
			if (!get(to, black).isWall()) {
				if (addition) {
					effectBoard<black>()[to.getIndex()].add(flag);
				} else {
					effectBoard<black>()[to.getIndex()].remove(flag);
				}
			}
		}
		if (!addition) {
			changeAround<true, true>(sq, board);
			changeAround<false, true>(sq, board);
		}
	}
	template void EffectBoard::change<true, true>(const Square& sq, const DirectionFlags& dirFlags, const Board& board);
	template void EffectBoard::change<true, false>(const Square& sq, const DirectionFlags& dirFlags, const Board& board);
	template void EffectBoard::change<false, true>(const Square& sq, const DirectionFlags& dirFlags, const Board& board);
	template void EffectBoard::change<false, false>(const Square& sq, const DirectionFlags& dirFlags, const Board& board);

	bool EffectBoard::equals(const EffectBoard& effectBoard) const {
		for (Square sq(Square::TOP_W); sq.valid(); sq.inc()) {
			if ((unsigned)blackEffectBoard[sq.getIndex()] != (unsigned)effectBoard.blackEffectBoard[sq.getIndex()] ||
					(unsigned)whiteEffectBoard[sq.getIndex()] != (unsigned)effectBoard.whiteEffectBoard[sq.getIndex()]) {
#if 0
				std::cout << sq.toString() << '\n';
#endif
				return false;
			}
		}
		return true;
	}

	std::string EffectBoard::toString(bool king) const {
		std::ostringstream oss;
		Square sq(Square::TOP);
		for (sq.leftmost(); !get(sq, true).isWall(); sq.leftmost(), sq.down()) {
			for (; !get(sq, true).isWall(); sq.right()) {
#if 0
				bool black = get(sq, true).longOrShortRange();
				bool white = get(sq, false).longOrShortRange();
				if (black && white) {
					oss << " X ";
				} else if (black) {
					oss << " b ";
				} else if (white) {
					oss << " w ";
				} else {
					oss << " * ";
				}
#else
				oss.width(3);
				DirectionFlags bflags = get(sq, true);
				DirectionFlags wflags = get(sq, false);
				if (!king) {
					bflags = bflags.getExcludeKing();
					wflags = wflags.getExcludeKing();
				}
				oss << ((int)Util::Bit::count(bflags.getInteger())
					- (int)Util::Bit::count(wflags.getInteger()));
#endif
			}
			oss << '\n';
		}
		return oss.str();
	}
}

