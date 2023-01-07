/*
 * hashMove.h
 *
 *  Created on: 2012/07/06
 *      Author: ryosuke
 */

#ifndef HASHMOVE_H_
#define HASHMOVE_H_

#include "../Shogi/move.h"

namespace Search {
	class HashMove {
	private:
		Shogi::Move hash1;
		Shogi::Move hash2;

	public:
		HashMove() {
			init();
		}

		HashMove(const HashMove& hashMove) {
			hash1 = hashMove.hash1;
			hash2 = hashMove.hash2;
		}

		void init() {
			hash1.setEmpty();
			hash2.setEmpty();
		}

		void update(const Shogi::Move& move) {
			if (hash1 != move) {
				hash2 = hash1;
				hash1 = move;
			}
		}

		const Shogi::Move& getHash1() const {
			return hash1;
		}

		void setHash1(const Shogi::Move& move) {
			this->hash1 = move;
		}

		const Shogi::Move& getHash2() const {
			return hash2;
		}

		void setHash2(const Shogi::Move& move) {
			this->hash2 = move;
		}

		operator Util::uint64() const {
			return ((Util::uint64)(unsigned)hash1 << 32)
					+ (Util::uint64)(unsigned)hash2;
		}
	};
}

#endif // HASHMOVE_H_
