/*
 * ttEntity.cpp
 *
 *  Created on: 2012/07/05
 *      Author: ryosuke
 */

#include "ttEntity.h"

namespace Table {
	using namespace Evaluates;

	bool TTEntity::update(Util::uint64 newHash,
			Evaluates::Value newValue,
			int newValueType,
			int newDepth, int ply,
			const Search::NodeStat& newStat,
			const Shogi::Move& move,
			unsigned newAge) {
		assert(newAge < AGE_MAX);
		assert(newDepth < (1<<20));
		assert(newValueType < (1<<2));
		assert((unsigned)newStat < (1<<4));

		if (newDepth < 0) { newDepth = 0; }

		if (isOk()) {
			assert(hash == newHash);
			// 深さが劣るものは登録させない。
			if (newDepth < (int)e.depth && e.age == newAge) {
				return false;
			}
		} else {
			hash = newHash;
			hashMove.init();
		}

		if (value >= Value::MATE) {
			if (newValueType == LOWER) {
				if (value < Value::MAX - ply) {
					value += ply;
				} else {
					value = Value::MAX;
				}
			}
		} else if (value <= -Value::MATE) {
			if (newValueType == UPPER) {
				if (value > Value::MIN + ply) {
					value -= ply;
				} else {
					value = Value::MIN;
				}
			}
		}

		value = newValue;
		e.valueType = newValueType;
		e.depth = (unsigned)newDepth;
		e.stat = (unsigned)newStat;
		if (!move.isEmpty()) { hashMove.update(move); }
		e.age = newAge;
		checkSum = generateCheckSum();

		return true;
	}

	void TTEntities::set(const TTEntity& entity) {
		unsigned l = lastAccess;
		for (unsigned i = 0; i < SIZE; i++) {
			const unsigned index = (l + i) % SIZE;
			if (entities[index].getHash() == entity.getHash()) {
				entities[index] = entity;
				lastAccess = index;
				return;
			}
		}
		l++;
		for (unsigned i = 0; i < SIZE; i++) {
			const unsigned index = (l + i) % SIZE;
			if (entities[index].isBroken() ||
					entities[index].getAge() != entity.getAge()) {
				entities[index] = entity;
				lastAccess = index;
				return;
			}
		}
		const unsigned index = l % SIZE;
		entities[index] = entity;
		lastAccess = index;
	}

	bool TTEntities::get(Util::uint64 hash, TTEntity& entity) {
		unsigned l = lastAccess;
		for (unsigned i = 0; i < SIZE; i++) {
			const unsigned index = (l + i) % SIZE;
			if (entities[index].getHash() == hash) {
				entity = entities[index];
				lastAccess = index;
				return true;
			}
		}
		return false;
	}
}
