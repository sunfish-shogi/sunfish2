/*
 * square.h
 *
 *  Created on: 2012/05/10
 *      Author: ryosuke
 */

#ifndef SQUARE_H_
#define SQUARE_H_

#include "piece.h"
#include "direction.h"

namespace Shogi {
	class Square {
	private:
		static const unsigned nextTable[];
		static const unsigned shortIndex[];
		unsigned square;

	public:
		static const unsigned UPPERMOST = 0x20;
		static const unsigned LOWERMOST = 0xA0;
		static const unsigned RIGHTMOST = 0x01;
		static const unsigned LEFTMOST = 0x09;
		static const unsigned MASK_FILE = 0x0F;
		static const unsigned MASK_RANK = 0xF0;
		static const unsigned SHIFT_RANK = 4;
		static const unsigned OFFSET = 0x10;
		static const unsigned WIDTH = 0x10;
		static const unsigned SIZE = 0xD0;
		static const unsigned NON = 0x00;
		static const unsigned TOP_W = 0x00;
		static const unsigned TOP = 0x21;
		static const unsigned END = 0xA9;
		static const unsigned END_W = 0xCF;
		static const unsigned INVALID = 0x100;
		static const unsigned FILE_NUM = 9;
		static const unsigned RANK_NUM = 9;

		Square() {
			square = TOP;
		}

		Square(int square) {
			this->square = square;
		}

		Square(unsigned file, unsigned rank) {
			square = file + (rank << SHIFT_RANK) + OFFSET;
		}

		Square(const Square& sq) {
			square = sq.square;
		}

		unsigned getShortIndex() const {
			return shortIndex[square];
		}

		void next() {
			square = nextTable[square];
		}

		void inc() {
			square++;
		}

		void right() {
			square--;
		}

		void left() {
			square++;
		}

		void up() {
			square -= WIDTH;
		}

		void down() {
			square += WIDTH;
		}

		void to(const Direction& dir) {
			square += (int)dir;
		}

		void to(const Direction& dir, int num) {
			square += (int)dir * num;
		}

		void operator+=(const Direction& dir) {
			square += (int)dir;
		}

		void operator+=(int dir) {
			square += (int)dir;
		}

		Square add(const Direction& dir) const {
			Square sq(*this);
			sq += dir;
			return sq;
		}

		Square add(int dir) const {
			Square sq(*this);
			sq += dir;
			return sq;
		}

		Square operator+(const Direction& dir) const {
			return add(dir);
		}

		Square operator+(int dir) const {
			return add(dir);
		}

		operator unsigned() const {
			return square;
		}

		void rightmost() {
			square = (square & MASK_RANK) | RIGHTMOST;
		}

		void leftmost() {
			square = (square & MASK_RANK) | LEFTMOST;
		}

		void uppermost() {
			square = (square & MASK_FILE) | UPPERMOST;
		}

		void lowermost() {
			square = (square & MASK_FILE) | LOWERMOST;
		}

		bool valid() const {
			return square <= END_W;
		}

		unsigned getIndex() const {
			return square;
		}

		unsigned getFile() const {
			return (square & MASK_FILE);
		}

		unsigned getRank() const {
			return ((square & MASK_RANK) - OFFSET) >> SHIFT_RANK;
		}

		bool is(unsigned file, unsigned rank) const {
			return square == Square(file, rank).square;
		}

		static bool isInside(unsigned file, unsigned rank) {
			return file >= 1 && file <= FILE_NUM && rank >= 1 && rank <= RANK_NUM;
		}

		bool isPromotableRank(bool black) const {
			if (black) {
				return square <= Square(9, 3).getIndex();
			} else {
				return square >= Square(1, 7).getIndex();
			}
		}

		bool isCompulsoryPromotion(const Piece& piece) const {
			switch (piece.getInteger()) {
			case Piece::BPAWN: case Piece::BLANCE:
				return square <= Square(9, 1).getIndex();
			case Piece::BKNIGHT:
				return square <= Square(9, 2).getIndex();
			case Piece::WPAWN: case Piece::WLANCE:
				return square >= Square(1, 9).getIndex();
			case Piece::WKNIGHT:
				return square >= Square(1, 8).getIndex();
			default:
				return false;
			}
		}

		/*
		 * 文字列をSquareに変換する。
		 * 不正な入力に対してはSquare(Square::INVALID)を返す。
		 * 但し"00"の場合にはSquare(Square::NON)を返す。
		 */
		static Square parse(const char* str) {
			if (str[0] == '\0' || str[1] == '\0' ||
					str[0] < '1' || str[0] > '9' ||
					str[1] < '1' || str[1] > '9') {
				if (str[0] == '0' && str[1] == '0') {
					return Square(NON);
				} else {
					return Square(INVALID);
				}
			}
			int file = str[0] - '0';
			int rank = str[1] - '0';
			return Square(file, rank);
		}

		std::string toString() const;
	};
}

#endif /* SQUARE_H_ */
