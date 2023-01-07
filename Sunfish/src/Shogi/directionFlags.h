/*
 * directionBit.h
 *
 *  Created on: 2012/05/16
 *      Author: ryosuke
 */

#ifndef DIRECTIONFLAGS_H_
#define DIRECTIONFLAGS_H_

#include "directionAndRange.h"
#include <string>
#include <sstream>
#include <iostream>

namespace Shogi {
	class DirectionFlags {
	private:
		static const unsigned int firstBit[];
		static const unsigned int direction[];
		static const unsigned int _dir2bit[];
		static const unsigned int* dir2bit;
		unsigned bits;

		static unsigned longRangeShift(unsigned bits) {
			return (bits << LONG_SHIFT) & LONG_MASK;
		}

	public:
		enum {
			NON               = 0x00000000,
			SHORT_RIGHT_UP    = 0x00000001,
			SHORT_UP          = 0x00000002,
			SHORT_LEFT_UP     = 0x00000004,
			SHORT_RIGHT       = 0x00000008,
			SHORT_LEFT        = 0x00000010,
			SHORT_RIGHT_DOWN  = 0x00000020,
			SHORT_DOWN        = 0x00000040,
			SHORT_LEFT_DOWN   = 0x00000080,
			SHORT_RIGHT_UP2   = 0x00000100,
			SHORT_LEFT_UP2    = 0x00000200,
			SHORT_RIGHT_DOWN2 = 0x00000400,
			SHORT_LEFT_DOWN2  = 0x00000800,
			LONG_RIGHT_UP     = 0x00001000,
			LONG_UP           = 0x00002000,
			LONG_LEFT_UP      = 0x00004000,
			LONG_RIGHT        = 0x00008000,
			LONG_LEFT         = 0x00010000,
			LONG_RIGHT_DOWN   = 0x00020000,
			LONG_DOWN         = 0x00040000,
			LONG_LEFT_DOWN    = 0x00080000,
			KING_LEFT_DOWN    = 0x00100000,
			KING_DOWN         = 0x00200000,
			KING_RIGHT_DOWN   = 0x00400000,
			KING_LEFT         = 0x00800000,
			KING_RIGHT        = 0x01000000,
			KING_LEFT_UP      = 0x02000000,
			KING_UP           = 0x04000000,
			KING_RIGHT_UP     = 0x08000000,
			WALL              = 0x10000000,

			LONG_SHIFT        = 12,
			KING_SHIFT        = 8,
			SHORT_MASK        = 0x00000fff,
			LONG_MASK         = 0x000ff000,
			KING_MASK         = 0x0ff00000,

			PIECE = SHORT_UP,
			LANCE = LONG_UP,
			KNIGHT = SHORT_RIGHT_UP2 | SHORT_LEFT_UP2,
			SILVER = SHORT_RIGHT_UP | SHORT_UP | SHORT_LEFT_UP | SHORT_RIGHT_DOWN | SHORT_LEFT_DOWN,
			GOLD = SHORT_RIGHT_UP | SHORT_UP | SHORT_LEFT_UP | SHORT_RIGHT | SHORT_LEFT | SHORT_DOWN,
			BISHOP = LONG_RIGHT_UP | LONG_LEFT_UP | LONG_RIGHT_DOWN | LONG_LEFT_DOWN,
			ROOK = LONG_UP | LONG_LEFT | LONG_RIGHT | LONG_DOWN,
			KING = SHORT_RIGHT_UP | SHORT_UP | SHORT_LEFT_UP | SHORT_RIGHT | SHORT_LEFT | SHORT_RIGHT_DOWN | SHORT_DOWN | SHORT_LEFT_DOWN
				| KING_RIGHT_UP | KING_UP | KING_LEFT_UP | KING_RIGHT | KING_LEFT | KING_RIGHT_DOWN | KING_DOWN | KING_LEFT_DOWN,
			HORSE = LONG_RIGHT_UP | LONG_LEFT_UP | LONG_RIGHT_DOWN | LONG_LEFT_DOWN | SHORT_UP | SHORT_RIGHT | SHORT_LEFT | SHORT_DOWN,
			DRAGON = LONG_UP | LONG_LEFT | LONG_RIGHT | LONG_DOWN | SHORT_RIGHT_UP  | SHORT_LEFT_UP | SHORT_RIGHT_DOWN | SHORT_LEFT_DOWN,

			BPIECE = PIECE,
			BLANCE = LANCE,
			BKNIGHT = KNIGHT,
			BSILVER = SILVER,
			BGOLD = GOLD,
			BBISHOP = BISHOP,
			BROOK = ROOK,
			BKING = KING,
			BHORSE = HORSE,
			BDRAGON = DRAGON,

			WPIECE = SHORT_DOWN,
			WLANCE = LONG_DOWN,
			WKNIGHT = SHORT_RIGHT_DOWN2 | SHORT_LEFT_DOWN2,
			WSILVER = SHORT_RIGHT_DOWN | SHORT_DOWN | SHORT_LEFT_DOWN | SHORT_RIGHT_UP | SHORT_LEFT_UP,
			WGOLD = SHORT_RIGHT_DOWN | SHORT_DOWN | SHORT_LEFT_DOWN | SHORT_RIGHT | SHORT_LEFT | SHORT_UP,
			WBISHOP = LONG_RIGHT_DOWN | LONG_LEFT_DOWN | LONG_RIGHT_UP | LONG_LEFT_UP,
			WROOK = LONG_DOWN | LONG_LEFT | LONG_RIGHT | LONG_UP,
			WKING = SHORT_RIGHT_DOWN | SHORT_DOWN | SHORT_LEFT_DOWN | SHORT_RIGHT | SHORT_LEFT | SHORT_RIGHT_UP | SHORT_UP | SHORT_LEFT_UP
				| KING_RIGHT_DOWN | KING_DOWN | KING_LEFT_DOWN | KING_RIGHT | KING_LEFT | KING_RIGHT_UP | KING_UP | KING_LEFT_UP,
			WHORSE = LONG_RIGHT_DOWN | LONG_LEFT_DOWN | LONG_RIGHT_UP | LONG_LEFT_UP | SHORT_DOWN | SHORT_RIGHT | SHORT_LEFT | SHORT_UP,
			WDRAGON = LONG_DOWN | LONG_LEFT | LONG_RIGHT | LONG_UP | SHORT_RIGHT_DOWN  | SHORT_LEFT_DOWN | SHORT_RIGHT_UP | SHORT_LEFT_UP,
		};

		DirectionFlags() : bits(0) {
		}

		DirectionFlags(unsigned bits) : bits(bits) {
		}

		DirectionFlags(const DirectionFlags& bit) : bits(bit.bits) {
		}

		DirectionFlags(const Direction& dir, bool longRange = false) {
			set(dir, longRange);
		}

		DirectionFlags(const DirectionAndRange& dir) {
			set(dir);
		}

		unsigned getInteger() const {
			return bits;
		}

		void set(unsigned bits) {
			this->bits = bits;
		}

		void set(const Direction& dir, bool longRange = false) {
			bits = dir2bit[(int)dir];
			if (longRange) {
				bits = longRangeShift(bits);
			}
		}

		void set(const DirectionAndRange& dir) {
			bits = dir2bit[(int)dir];
			if (dir.isLongRange()) {
				bits = longRangeShift(bits);
			}
		}

		void set(const DirectionFlags& bit) {
			bits = bit.bits;
		}

		void add(const DirectionFlags& bit) {
			bits |= bit.bits;
		}

		void add(const Direction& dir) {
			bits |= DirectionFlags(dir).bits;
		}

		void add(const DirectionAndRange& dir) {
			bits |= DirectionFlags(dir).bits;
		}

		void remove(const DirectionFlags& bit) {
			bits &= ~DirectionFlags(bit).bits;
		}

		void remove(const Direction& dir) {
			bits &= ~DirectionFlags(dir).bits;
		}

		void remove(const DirectionAndRange& dir) {
			bits &= ~DirectionFlags(dir).bits;
		}

		bool check(const DirectionFlags& bit) const {
			return bits & bit.bits;
		}

		bool check(const DirectionAndRange& dir) const {
			return bits & DirectionFlags(dir).bits;
		}

		bool check(const Direction& dir, bool longRange = false) const {
			return bits & DirectionFlags(dir, longRange).bits;
		}

		bool isZero() const {
			return bits == NON;
		}

		bool isNonZero() const {
			return !isZero();
		}

		bool isSubset(const DirectionFlags& flags) const {
			return (bits & flags.bits) == flags.bits;
		}

		bool isSubsetOf(const DirectionFlags& flags) const {
			return (bits & flags.bits) == bits;
		}

		bool longOrShortRange() const {
			return (bits & (LONG_MASK | SHORT_MASK)) != 0U;
		}

		bool longRange() const {
			return (bits & LONG_MASK) != 0U;
		}

		bool shortRange() const {
			return (bits & SHORT_MASK) != 0U;
		}

		bool king() const {
			return (bits & KING_MASK) != 0U;
		}

		bool isWall() const {
			return bits == WALL;
		}

		bool isKing() const {
			return bits & KING_MASK;
		}

		bool isLongRange() const {
			return bits & LONG_MASK;
		}

		bool isShortRange() const {
			return bits & SHORT_MASK;
		}

		bool isPlural() const {
			return (bits & (bits-1)) != 0U;
		}

		DirectionFlags isAttackedBy(const DirectionFlags attacker) const {
			return DirectionFlags((bits >> KING_SHIFT) & attacker.bits & LONG_MASK);
		}

		DirectionFlags getKingOnly() const {
			return DirectionFlags(bits & KING_MASK);
		}

		DirectionFlags getExcludeKing() const {
			return DirectionFlags(bits & ~KING_MASK);
		}

		DirectionFlags getLongRangeOnly() const {
			return DirectionFlags(bits & LONG_MASK);
		}

		DirectionFlags getShortRangeOnly() const {
			return DirectionFlags(bits & SHORT_MASK);
		}

		DirectionFlags getLongRangeAndKing() const {
			return DirectionFlags(bits & (LONG_MASK | KING_MASK));
		}

		DirectionFlags pop() {
			unsigned mask = bits-1;
			unsigned temp = bits;
			bits = bits & mask;
			return DirectionFlags(temp & ~mask);
		}

		operator unsigned() const {
			return bits;
		}

		operator bool() const {
			return bits != 0U;
		}

		unsigned numberOfFlags() const {
			return 0;
		}

		const DirectionFlags& operator=(unsigned bits) {
			this->bits = bits;
			return *this;
		}

		Direction toDirection() const {
			int b;
			if( bits == 0U ){ b = 0; }
			else if( ( b = firstBit[ bits     &0xff] ) != 0 ){ }
			else if( ( b = firstBit[(bits>> 8)&0xff] ) != 0 ){ b += 8; }
			else if( ( b = firstBit[(bits>>16)&0xff] ) != 0 ){ b += 16; }
			else {     b = firstBit[(bits>>24)     ] + 24; }
			return Direction(direction[b] );
		}

		DirectionAndRange toDirectionAndRange() const {
			return DirectionAndRange(toDirection(), !isShortRange() );
		}

		std::string toString() const {
			std::ostringstream oss;
			oss << "0x";
			oss << oss.hex << bits;
			return oss.str();
		}
	};
}

#endif /* DIRECTIONFLAGS_H_ */
