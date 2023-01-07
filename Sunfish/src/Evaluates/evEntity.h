/*
 * evEntity.h
 *
 *  Created on: 2012/07/09
 *      Author: ryosuke
 */

#ifndef EVENTITY_H_
#define EVENTITY_H_

#include "param.h"

namespace Evaluates {
	class EvEntity {
	private:
		static const int VALUE_INFL = 1U << (21 - 1); // TODO: magic number
		static const Util::uint64 VALUE_MASK = (Util::uint64)((1U << 21) - 1U); // TODO: magic number
		static const Util::uint64 HASH_MASK = ~VALUE_MASK;
		Util::uint64 data;

	public:
		EvEntity() {
			init();
		}

		void init() {
			data = U64(0);
		}

		void init(unsigned) {
			init();
		}

		bool get(Util::uint64 hash, Value& value) const {
			Util::uint64 temp = data;
			if ((temp & HASH_MASK) == (hash & HASH_MASK)) {
				value = (int)(temp & VALUE_MASK) - VALUE_INFL;
				return true;
			}
			return false;
		}

		void set(Util::uint64 hash, const Value& value) {
			Util::uint64 temp = (hash & HASH_MASK) | ((value + VALUE_INFL) & VALUE_MASK);
			data = temp;
		}
	};
}

#endif // EVENTITY_H_
