/*
 * record.cpp
 *
 *  Created on: 2013/01/13
 *      Author: ryosuke
 */

#include "record.h"

namespace Records {
	using namespace Util;

	bool Record::isRepetition(bool full) const {
		int cnt = 0;
		uint64 curHash = hashStack[cur].positionHash;
		for (int i = cur - 1; i >= 0; i--) {
			if (curHash == hashStack[i].positionHash) {
				if (!full || cnt >= 2) {
					return true;
				}
				cnt++;
			}
		}
		return false;
	}
}
