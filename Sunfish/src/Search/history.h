/*
 * history.h
 *
 *  Created on: 2012/07/14
 *      Author: ryosuke
 */

#ifndef HISTORY_H_
#define HISTORY_H_

#include "../Shogi/move.h"

namespace Search {
	class History {
	private:
		static const unsigned FROM_BOARD = Shogi::Square::END + 1;
		static const unsigned FROM_HAND = Shogi::Piece::ROOK + 1;
		static const unsigned FROM = FROM_BOARD + FROM_HAND;
		static const unsigned TO = Shogi::Square::END + 1;
		Util::uint64 (*hist)[TO];

		unsigned from(const Shogi::Move& move) const {
			if (move.isHand()) {
				return FROM_BOARD + move.getPiece().getTurnedBlack();
			} else {
				return move.getFrom();
			}
		}

	public:
		static const unsigned SCALE = 0x100;

		History() {
			hist = new Util::uint64[FROM][TO];
			assert(hist != NULL);
		}

		virtual ~History() {
			delete[] hist;
		}

		void clear() {
			memset((void*)hist, 0, sizeof(Util::uint64) * FROM * TO);
		}

		void reduce() {
			for (unsigned from = 0; from < FROM; from++) {
				for (unsigned to = 0; to < TO; to++) {
					hist[from][to] = (hist[from][to] >> 3) & ~U64(0xe0000000);
				}
			}
		}

		void add(const Shogi::Move& move, int appear, int good) {
			unsigned index = from(move);
			Util::uint64 h = hist[index][move.getTo()];
			Util::uint64 d = ((Util::uint64)appear << 32) + good;
			if (h >= U64(0x0100000000000000) - d) {
				h = (h >> 1) & ~U64(0x80000000);
			}
			hist[index][move.getTo()] = h + d;
		}

		unsigned get(const Shogi::Move& move) const {
			unsigned index = from(move);
			assert(index < FROM);
			assert(move.getTo() < TO);
			Util::uint64 h = hist[index][move.getTo()];
			unsigned a = (unsigned)(h >> 32);
			unsigned g = (unsigned)h;
			return g * SCALE / (a + 1);
		}
	};
}

#endif // HISTORY_H_
