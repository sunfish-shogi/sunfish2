/*
 * hand.h
 *
 *  Created on: 2012/05/12
 *      Author: ryosuke
 */

#ifndef HAND_H_
#define HAND_H_

#include "piece.h"
#include <cstdlib>
#include <cstring>
#include <iostream>

namespace Shogi {
	class Hand {
	private:
		int hand[Piece::ROOK+1];

	public:
		static const int MAX = 18;

		Hand() {
			init();
		}

		Hand(const Hand& hand) {
			init(hand);
		}

		void init() {
			memset((void*)hand, 0, sizeof(hand));
		}

		void init(const Hand& source) {
			memcpy((void*)hand, (const void*)source.hand, sizeof(hand));
		}

		void set(const Piece& piece, int num) {
			hand[piece.getTurnedBlack()] = num;
		}

		void inc(const Piece& piece) {
			hand[piece.getTurnedBlack()]++;
		}

		void dec(const Piece& piece) {
			hand[piece.getTurnedBlack()]--;
		}

		int get(const Piece& piece) const {
			return hand[piece.getTurnedBlack()];
		}

		std::string toString() const;

		std::string toStringCsa() const;

		bool writeBinary(std::ostream& out) const {
			out.write((const char*)hand, sizeof(hand));
			return !out.fail();
		}

		bool readBinary(std::istream& in) {
			in.read((char*)hand, sizeof(hand));
			return !in.fail();
		}
	};
}

#endif /* HAND_H_ */
