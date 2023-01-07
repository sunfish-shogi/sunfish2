/*
 * Direction.h
 *
 *  Created on: 2012/05/13
 *      Author: ryosuke
 */

#ifndef DIRECTION_H_
#define DIRECTION_H_

namespace Shogi {
	class Direction {
	private:
		int direction;

	public:
		enum {
			LEFT_UP2   = -31,             RIGHT_UP2   = -33,
			LEFT_UP    = -15, UP   = -16, RIGHT_UP    = -17,
			LEFT       = 1,   NON  = 0,   RIGHT       = -1,
			LEFT_DOWN  = 17,  DOWN = 16,  RIGHT_DOWN  = 15,
			LEFT_DOWN2 = 33,              RIGHT_DOWN2 = 31,

			LEFT_UP2_R   = 31,                RIGHT_UP2_R   = 33,
			LEFT_UP_R    = 15,  UP_R   = 16,  RIGHT_UP_R    = 17,
			LEFT_R       = 1,   NON_R  = 0,   RIGHT_R       = -1,
			LEFT_DOWN_R  = -17, DOWN_R = -16, RIGHT_DOWN_R  = -15,
			LEFT_DOWN2_R = -33,               RIGHT_DOWN2_R = -31,
		};

		Direction(const Direction& dir) {
			direction = dir.direction;
		}

		Direction(int direction = NON) {
			this->direction = direction;
		}

		operator int() const {
			return this->direction;
		}

		Direction reverse() const {
			return Direction(-direction);
		}

		const Direction& operator=(const Direction& dir) {
			direction = dir.direction;
			return *this;
		}
	};
}

#endif /* DIRECTION_H_ */
