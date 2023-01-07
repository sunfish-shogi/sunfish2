/*
 * searchCounter.h
 *
 *  Created on: 2013/02/24
 *      Author: ryosuke
 */

#ifndef SEARCHCOUNTER_H_
#define SEARCHCOUNTER_H_

#include "../Log/logger.h"
#include "../Util/int.h"

namespace Search {
	struct SearchCounter {
		Util::uint64 nodes;
		Util::uint64 hashPruning;
		Util::uint64 nullMovePruning;
		Util::uint64 futilityPruning;
		Util::uint64 exFutilityPruning;
		Util::uint64 shekPruning;

		SearchCounter(const SearchCounter& source) {
			nodes = source.nodes;
			hashPruning = source.hashPruning;
			nullMovePruning = source.nullMovePruning;
			futilityPruning = source.futilityPruning;
			exFutilityPruning = source.exFutilityPruning;
			shekPruning = source.shekPruning;
		}

		SearchCounter() {
			init();
		}

		void init() {
			nodes = U64(0);
			hashPruning = U64(0);
			nullMovePruning = U64(0);
			futilityPruning = U64(0);
			exFutilityPruning = U64(0);
			shekPruning = U64(0);
		}

		const SearchCounter& operator+=(const SearchCounter& counter) {
			nodes += counter.nodes;
			hashPruning += counter.hashPruning;
			nullMovePruning += counter.nullMovePruning;
			futilityPruning += counter.futilityPruning;
			exFutilityPruning += counter.exFutilityPruning;
			shekPruning += counter.shekPruning;
			return *this;
		}
	};
}

#endif // SEARCHCOUNTER_H_
