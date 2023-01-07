/*
 * moveGenerator.h
 *
 *  Created on: 2012/05/13
 *      Author: ryosuke
 */

#ifndef MOVEGENERATOR_H_
#define MOVEGENERATOR_H_

#include <boost/range/algorithm.hpp>
#include "position.h"
#include "move.h"

namespace Shogi {
	class MoveGenerator {
	public:
		static const unsigned MAX_MOVES_NUM = 1024;

	private:
		const Position& pos;
		unsigned num;
		unsigned curr;
		Move moves[MAX_MOVES_NUM];

		template <bool genCap, bool genNocap, bool full>
		unsigned generate();

		template <bool black, bool genCap, bool genNocap, bool genPro, bool full>
		void generateOnBoard();

		template <bool black, bool oneStep, bool promotable, bool genCap, bool genNocap, bool genPro, bool full>
		void generateStraight(const Piece piece, const Square from, const Direction dir, const Direction pin);

		template <bool black>
		bool generateOneMove(const Piece piece, const Square from, const Square to) {
			if (!to.isCompulsoryPromotion(piece)) {
				moves[num++] = Move(from, to, false, false, piece);
				return true;
			}
			return false;
		}

		template <bool black>
		bool generateOneMovePro(const Piece piece, const Square from, const Square to) {
			if (to.isPromotableRank(black) || from.isPromotableRank(black)) {
				moves[num++] = Move(from, to, true, false, piece);
				return true;
			}
			return false;
		}

		template <bool black, bool check, bool genCap, bool genNocap, bool genCheckOnly>
		void generateKing();

		template <bool black, bool check, bool genCap, bool genNocap, bool genCheckOnly>
		void generateKingDirection(const Direction& dir);

		template <bool black>
		void generateDrop();

		template <bool black, unsigned piece>
		void generateDropPieces();

		template <bool black, bool genCap, bool genNocap, bool genCheckOnly, bool full>
		void generateEvasion();

		template <bool black, bool genCheckOnly, bool full>
		void generateEvasionOnBoard(Square to);

		template <bool black, unsigned piece, bool genCheckOnly>
		void generateEvasionDrop(Square to);

		template <bool black>
		void generateCheckOnBoard();

		template <bool black, bool direct>
		void generateCheck(Square from, Square to, Piece piece);

		template <bool black>
		void generateCheckDrop();

		template <bool black, unsigned piece, bool longRange>
		void generateCheckDropStraight(Direction dir);

		static bool promotionOnly(const Piece& piece) {
			return piece == Piece::BPAWN || piece == Piece::BBISHOP ||
					piece == Piece::BROOK || piece == Piece::WPAWN ||
					piece == Piece::WBISHOP || piece == Piece::WROOK;
		}

	public:
		MoveGenerator(const Position& pos) : pos(pos) {
			clear();
		}

		const Position& getPosition() const {
			return pos;
		}

		void clear() {
			num = 0;
			curr = 0;
		}

		unsigned generate() {
			return generate<true, true, false>();
		}

		unsigned generateFull() {
			return generate<true, true, true>();
		}

		unsigned generateTardy();

		unsigned generateCapture() {
			return generate<true, false, false>();
		}

		unsigned generateCaptureFull() {
			return generate<true, false, true>();
		}

		unsigned generateNocapture() {
			return generate<false, true, false>();
		}

		unsigned generateNocaptureFull() {
			return generate<false, true, true>();
		}

		unsigned generateTactical();

		unsigned generateCheck();

		unsigned add(const Move& move) {
			moves[num++] = move;
			return num;
		}

		unsigned getNumber() const {
			return num;
		}

		void setCurrent(unsigned curr) {
			this->curr = curr;
		}

		unsigned getCurrent() const {
			return curr;
		}

		void sort() {
			sort(0, num);
		}

		void sort(int begin) {
			sort(begin, num - begin);
		}

		void sort(int begin, int size);

		void insertBefore(int from, int to) {
			Move move = moves[from];
			for (int i = from; i > to; i--) {
				moves[i] = moves[i-1];
			}
			moves[to] = move;
		}

		void removeUnstable(int index) {
			moves[index] = moves[num-1];
			num--;
		}

		int removeUnstable(const Move& move);

		void removeAfter(int index) {
			num = index;
		}

		bool swapTop(Move move) {
			if (num > 0 && moves[0] == move) {
				return true;
			}
			for (int i = 1; (unsigned)i < num; i++) {
				if (moves[i] == move) {
					moves[i] = moves[0];
					moves[0] = move;
					return true;
				}
			}
			return false;
		}

		bool equals(const MoveGenerator gen) const;

		const Move* next() {
			if (curr < num) {
				return &moves[curr++];
			} else {
				return NULL;
			}
		}

		const Move* get() const {
			if (curr > 0) {
				return &moves[curr-1];
			} else {
				return NULL;
			}
		}

		const Move& get(unsigned index) const {
			return moves[index];
		}
	};
}

#endif /* MOVEGENERATOR_H_ */
