/*
 * directionAndRange.h
 *
 *  Created on: 2012/05/19
 *      Author: ryosuke
 */

#ifndef DIRECTIONANDRANGE_H_
#define DIRECTIONANDRANGE_H_

#include "direction.h"

namespace Shogi {
	class DirectionAndRange : public Direction {
	private:
		bool longRange;

	public:
		DirectionAndRange(const DirectionAndRange& dir) : Direction(dir), longRange(dir.longRange) {
		}

		DirectionAndRange(const Direction& dir, bool longRange = false) : Direction(dir), longRange(longRange) {
		}

		DirectionAndRange(int direction = NON, bool longRange = false) : Direction(direction), longRange(longRange) {
		}

		void setLongRange() {
			longRange = true;
		}

		void setShortRange() {
			longRange = false;
		}

		bool isLongRange() const {
			return longRange;
		}

		bool isShortRange() const {
			return !isLongRange();
		}

		const DirectionAndRange& operator=(const DirectionAndRange& dir) {
			operator=(dir);
			longRange = dir.longRange;
			return *this;
		}
	};
}

#endif /* DIRECTIONANDRANGE_H_ */
