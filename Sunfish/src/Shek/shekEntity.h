/*
 * shekEntity.h
 *
 *  Created on: 2012/11/03
 *      Author: ryosuke
 */

#ifndef SHEKENTITY_H_
#define SHEKENTITY_H_

#include <cassert>
#include "../Log/logger.h"
#include "handSet.h"

namespace Shek {
	class ShekEntity {
	private:
		Util::uint64 hash;
		HandSet handSet;
		unsigned char count;
#ifndef NDEBUG
		int num;
#endif // NDEBUG
		bool blackTurn;

	public:
		void init(Util::uint64 none) {
			hash = none;
		}

		ShekStat check(const HandSet& handSet, bool blackTurn) const {
			ShekStat stat = handSet.compareTo(this->handSet, blackTurn);
			if (this->blackTurn != blackTurn) {
				if (stat == EQUAL) {
					stat = SUPERIOR;
				} else if (stat == INFERIOR) {
					stat = NONE;
				}
			}
			return stat;
		}

		void set(Util::uint64 hash,
				const HandSet& handSet, bool blackTurn
#ifndef NDEBUG
				, int num
#endif // NDEBUG
				) {
			this->hash = hash;
			this->handSet = handSet;
			this->blackTurn = blackTurn;
#ifndef NDEBUG
			this->num = num;
#endif // NDEBUG
			count = 0;
		}

		void add() {
			count++;
		}

		void unset(Util::uint64 none) {
			if (count != 0) {
				count--;
			} else {
				hash = none;
			}
		}

		Util::uint64 getHash() const {
			return hash;
		}

#ifndef NDEBUG
		void debugPrint(const HandSet& handSet, bool blackTurn) const {
			Log::debug << "********** SHEK DEBUG **********\n";
			Log::debug << "num=[" << num << "]\n";
			Log::debug << "check=[" << check(handSet, blackTurn) << "]\n";
			Log::debug << "handSet(current)=[" << handSet.toString() << "]\n";
			Log::debug << "handSet=[" << this->handSet.toString() << "]\n";
			Log::debug << "blackTurn(current)=[" << blackTurn << "]\n";
			Log::debug << "blackTurn=[" << this->blackTurn << "]\n";
			Log::debug << "********************************\n";
		}
#endif
	};

	class ShekEntities {
	private:
		static const unsigned SIZE = 4;
		Util::uint64 none;
		ShekEntity entities[SIZE];

	public:
		void init(unsigned key) {
			none = key + 1; // 絶対にぶつからない値
			for (unsigned i = 0; i < SIZE; i++) {
				entities[i].init(none);
			}
		}

		ShekStat check(Util::uint64 hash,
				const HandSet& handSet, bool blackTurn) const {
			for (unsigned i = 0; i < SIZE; i++) {
				if (entities[i].getHash() == hash) {
					return entities[i].check(handSet, blackTurn);
				}
			}
			return NONE;
		}

		void set(Util::uint64 hash,
				const HandSet& handSet, bool blackTurn
#ifndef NDEBUG
				, int num
#endif // NDEBUG
				) {
			for (unsigned i = 0; i < SIZE; i++) {
				if (entities[i].getHash() == hash) {
					entities[i].add();
					return;
				}
			}
			for (unsigned i = 0; i < SIZE; i++) {
				if (entities[i].getHash() == none) {
					entities[i].set(hash, handSet, blackTurn
#ifndef NDEBUG
							, num
#endif // NDEBUG
							);
					return;
				}
			}
		}

		void unset(Util::uint64 hash) {
			for (unsigned i = 0; i < SIZE; i++) {
				if (entities[i].getHash() == hash) {
					entities[i].unset(none);
					return;
				}
			}
		}

#ifndef NDEBUG
		void debugPrint(Util::uint64 hash,
				const HandSet& handSet, bool blackTurn) const {
			for (unsigned i = 0; i < SIZE; i++) {
				if (entities[i].getHash() == hash) {
					entities[i].debugPrint(handSet, blackTurn);
					return;
				}
			}
		}
#endif
	};
}
#endif // SHEKENTITY_H_
