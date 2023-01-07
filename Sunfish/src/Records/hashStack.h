/*
 * hashStack.h
 *
 *  Created on: 2013/02/19
 *      Author: ryosuke
 */

#ifndef HASH_STACK
#define HASH_STACK

#include "../Shek/handSet.h"
#include "../Log/logger.h"

namespace Records {
	struct HashData {
		Util::uint64 positionHash;
		Util::uint64 boardHash;
		Util::uint64 handHash;
		Util::uint64 turnHash;
		Shek::HandSet handSet;
		bool blackTurn;
		bool check;
	};

	struct HashStack {
		const HashData* stack;
		int size;
		HashStack(const HashStack& hashStack) {
			this->stack = hashStack.stack;
			this->size = hashStack.size;
		}
		HashStack(const HashData* stack, int size) {
			this->stack = stack;
			this->size = size;
		}
		static HashStack nan() {
			return HashStack(NULL, 0);
		}
	};
}

#endif // HASH_STACK
