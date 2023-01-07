/*
 * ttEntity.h
 *
 *  Created on: 2012/07/05
 *      Author: ryosuke
 */

#ifndef TTENTITY_H_
#define TTENTITY_H_

#include "../Util/int.h"
#include "../Evaluates/value.h"
#include "../Search/hashMove.h"
#include "../Search/nodeStat.h"
#include <cassert>

namespace Table {
	class TTEntity {
	private:
		unsigned checkSum;
		struct {
			// little elements
			unsigned age : 8;
			unsigned depth : 18;
			unsigned valueType : 2;
			unsigned stat : 4;
		} e;
		Util::uint64 hash;
		Evaluates::Value value;
		Search::HashMove hashMove;

		unsigned generateCheckSum() const {
			return (unsigned)hash
					^ (unsigned)(hash >> 32)
					^ (unsigned)(int)value
					^ (unsigned)hashMove.getHash1()
					^ (unsigned)hashMove.getHash2()
					^ (unsigned)e.age
					^ ((unsigned)e.depth << 8)
					^ ((unsigned)e.valueType << 28)
					^ ((unsigned)e.stat << 30)
					;
		}

		bool update(Util::uint64 newHash,
				Evaluates::Value newValue,
				int newValueType,
				int newDepth, int ply,
				const Search::NodeStat& newStat,
				const Shogi::Move& move,
				unsigned newAge);

	public:
		static const unsigned AGE_MAX = 1U << 8;

		enum {
			EXACT = 0,
			UPPER, // 1
			LOWER, // 2
		};

		TTEntity() {
			init();
		}

		void init() {
			checkSum = generateCheckSum() + 1;
		}

		bool update(Util::uint64 newHash,
				Evaluates::Value alpha,
				Evaluates::Value beta,
				Evaluates::Value newValue,
				int newDepth, int ply,
				const Search::NodeStat& newStat,
				const Shogi::Move& move,
				unsigned newAge) {
			int newValueType;
			if (newValue >= beta) {
				newValueType = LOWER;
			} else if (newValue <= alpha) {
				newValueType = UPPER;
			} else {
				newValueType = EXACT;
			}
			return update(newHash, newValue, newValueType,
					newDepth, ply, newStat, move, newAge);
		}

		bool isOk() const {
			return checkSum == generateCheckSum();
		}

		bool isBroken() const {
			return !isOk();
		}

		bool is(Util::uint64 hash) const {
			return this->hash == hash && isOk();
		}

		bool isSuperior(int curDepth) const {
			using namespace Evaluates;
			if ((int)e.depth >= curDepth) {
				return true;
			}
			if (value >= Value::MATE && e.valueType == LOWER) {
				return true;
			}
			if (value <= -Value::MATE && e.valueType == UPPER) {
				return true;
			}
			return false;
		}

		Util::uint64 getHash() const {
			return hash;
		}

		Evaluates::Value getValue(int ply) const {
			using namespace Evaluates;
			if (value >= Value::MATE) {
				if (e.valueType == LOWER) { return value - ply; }
			} else if (value <= -Value::MATE) {
				if (e.valueType == UPPER) { return value + ply; }
			}
			return value;
		}

		unsigned getValueType() const {
			return e.valueType;
		}

		int getDepth() const {
			return (int)e.depth;
		}

		const Search::NodeStat getStat() const {
			return Search::NodeStat(e.stat);
		}

		const Search::HashMove getHashMove() const {
			return hashMove;
		}

		unsigned getAge() const {
			return e.age;
		}
	};

	class TTEntities {
	private:
		static const unsigned SIZE = 4;
		TTEntity entities[SIZE];
		volatile unsigned lastAccess;

	public:
		TTEntities() : lastAccess(0) {
		}

		void init(unsigned) {
			for (unsigned i = 0; i < SIZE; i++) {
				entities[i].init();
			}
		}

		void set(const TTEntity& entity);

		bool get(Util::uint64 hash, TTEntity& entity);
	};
}

#endif // TTENTITY_H_
