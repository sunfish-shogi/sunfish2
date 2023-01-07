/*
 * piece.h
 *
 *  Created on: 2012/05/10
 *      Author: ryosuke
 */

#ifndef PIECE_H_
#define PIECE_H_

#include "directionFlags.h"
#include <string>
#include <iostream>

namespace Shogi {
	class Piece {
	private:
		unsigned piece;
		static const char pieceName[][4];
		static const char pieceNameS[][4];
		static const char UNKNOWN_NAME[];
		static const DirectionFlags direction[];
		static const int _diff2dir[];
		static const int* diff2dir;

	public:
		enum {
			EMPTY = 0,

			PROMOTE = 0x08, // 8 (mask)

			TURN = 0x10,    // 16 (mask)

			WALL = 0x20,    // 32

			PIECE = 0x0f,   // 15 (mask)

			PAWN = 1,       // 1
			LANCE,          // 2
			KNIGHT,         // 3
			SILVER,         // 4
			GOLD,           // 5
			BISHOP,         // 6
			ROOK,           // 7
			KING,           // 8
			TOKIN,          // 9
			PLANCE,         // 10
			PKNIGHT,        // 11
			PSILVER,        // 12

			HORSE = 14,     // 14
			DRAGON,         // 15

			BPAWN = 1,      // 1
			BLANCE,         // 2
			BKNIGHT,        // 3
			BSILVER,        // 4
			BGOLD,          // 5
			BBISHOP,        // 6
			BROOK,          // 7
			BKING,          // 8
			BTOKIN,         // 9
			BPLANCE,        // 10
			BPKNIGHT,       // 11
			BPSILVER,       // 12

			BHORSE = 14,    // 14
			BDRAGON,        // 15

			WPAWN = 17,     // 17
			WLANCE,         // 18
			WKNIGHT,        // 19
			WSILVER,        // 20
			WGOLD,          // 21
			WBISHOP,        // 22
			WROOK,          // 23
			WKING,          // 24
			WTOKIN,         // 25
			WPLANCE,        // 26
			WPKNIGHT,       // 27
			WPSILVER,       // 28

			WHORSE = 30,    // 30
			WDRAGON,        // 31
		};

		Piece() {
			piece = EMPTY;
		}

		Piece(unsigned piece) {
			this->piece = piece;
		}

		Piece(const Piece& p) {
			piece = p.piece;
		}

		void promote() {
			piece |= PROMOTE;
		}

		void unPromote() {
			piece &= ~PROMOTE;
		}

		void turnBlack() {
			piece &= PIECE;
		}

		void turnWhite() {
			piece |= TURN;
		}

		void turn() {
			piece ^= TURN;
		}

		Piece getPromoted() const {
			Piece p(*this);
			p.promote();
			return p;
		}

		Piece getUnpromoted() const {
			Piece p(*this);
			p.unPromote();
			return p;
		}

		Piece getTurnedBlack() const {
			Piece p(*this);
			p.turnBlack();
			return p;
		}

		Piece getTurnedWhite() const {
			Piece p(*this);
			p.turnWhite();
			return p;
		}

		Piece getTurned() const {
			Piece p(*this);
			p.turn();
			return p;
		}

		const Piece& operator=(const Piece& p) {
			piece = p.piece;
			return *this;
		}

		bool is(unsigned piece) {
			return this->piece == piece;
		}

		operator unsigned() const {
			return piece;
		}

		void toNext() {
			piece++;
		}

		void toPrev() {
			piece--;
		}

		unsigned getInteger() const {
			return piece;
		}

		bool isPromoted() const {
			return piece & PROMOTE;
		}

		bool isPromotable() const {
			return !isPromoted() && !isKing() && (piece&PIECE)!=GOLD;
		}

		bool isEmpty() const {
			return piece == EMPTY;
		}

		bool isWall() const {
			return piece == WALL;
		}

		bool isBlack() const {
			return !isWhite() && (piece & PIECE);
		}

		bool isWhite() const {
			return piece & TURN;
		}

		bool isBlackMovable() const {
			return isWhite() || isEmpty();
		}

		bool isWhiteMovable() const {
			return (piece & (WALL | TURN)) == 0U;
		}

		template <bool black>
		bool isKing() const {
			if (black) {
				return piece == BKING;
			} else {
				return piece == WKING;
			}
		}

		bool isKing() const {
			return (piece&PIECE) == KING;
		}

		DirectionFlags getMovableDirection() const {
			return direction[piece];
		}

		static Piece parseCsa(const char* str);

		std::string toString() const {
			if (!isWhite()) {
				return ' ' + toStringNameOnly();
			} else {
				return 'v' + toStringNameOnly();
			}
		}

		std::string toStringCsa() const {
			if (isBlack()) {
				return '+' + toStringNameOnly();
			} else if(isWhite()) {
				return '-' + toStringNameOnly();
			} else {
				return ' ' + toStringNameOnly();
			}
		}

		std::string toStringNameOnly() const {
			if (piece <= WDRAGON) {
				return pieceName[piece&PIECE];
			} else {
				return UNKNOWN_NAME;
			}
		}

		bool writeBinary(std::ostream& out) const {
			out.write((const char*)&piece, sizeof(piece));
			return true;
		}

		bool readBinary(std::istream& in) {
			in.read((char*)&piece, sizeof(piece));
			return !in.fail();
		}
	};
}

#endif /* PIECE_H_ */
