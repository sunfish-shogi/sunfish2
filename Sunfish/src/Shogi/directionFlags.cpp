/*
 * directionBit.cpp
 *
 *  Created on: 2012/05/16
 *      Author: ryosuke
 */

#include "directionFlags.h"

namespace Shogi {
	const unsigned int DirectionFlags::firstBit[] = {
			 0, 1, 2, 1, 3, 1, 2, 1, 4, 1, 2, 1, 3, 1, 2, 1, 5, 1, 2, 1,
			 3, 1, 2, 1, 4, 1, 2, 1, 3, 1, 2, 1, 6, 1, 2, 1, 3, 1, 2, 1,
			 4, 1, 2, 1, 3, 1, 2, 1, 5, 1, 2, 1, 3, 1, 2, 1, 4, 1, 2, 1,
			 3, 1, 2, 1, 7, 1, 2, 1, 3, 1, 2, 1, 4, 1, 2, 1, 3, 1, 2, 1,
			 5, 1, 2, 1, 3, 1, 2, 1, 4, 1, 2, 1, 3, 1, 2, 1, 6, 1, 2, 1,
			 3, 1, 2, 1, 4, 1, 2, 1, 3, 1, 2, 1, 5, 1, 2, 1, 3, 1, 2, 1,
			 4, 1, 2, 1, 3, 1, 2, 1, 8, 1, 2, 1, 3, 1, 2, 1, 4, 1, 2, 1,
			 3, 1, 2, 1, 5, 1, 2, 1, 3, 1, 2, 1, 4, 1, 2, 1, 3, 1, 2, 1,
			 6, 1, 2, 1, 3, 1, 2, 1, 4, 1, 2, 1, 3, 1, 2, 1, 5, 1, 2, 1,
			 3, 1, 2, 1, 4, 1, 2, 1, 3, 1, 2, 1, 7, 1, 2, 1, 3, 1, 2, 1,
			 4, 1, 2, 1, 3, 1, 2, 1, 5, 1, 2, 1, 3, 1, 2, 1, 4, 1, 2, 1,
			 3, 1, 2, 1, 6, 1, 2, 1, 3, 1, 2, 1, 4, 1, 2, 1, 3, 1, 2, 1,
			 5, 1, 2, 1, 3, 1, 2, 1, 4, 1, 2, 1, 3, 1, 2, 1,
	};

	const unsigned int DirectionFlags::direction[] = {
			Direction::NON,
			// 短い利き
			Direction::RIGHT_UP,
			Direction::UP,
			Direction::LEFT_UP,
			Direction::RIGHT,
			Direction::LEFT,
			Direction::RIGHT_DOWN,
			Direction::DOWN,
			Direction::LEFT_DOWN,
			Direction::RIGHT_UP2,
			Direction::LEFT_UP2,
			Direction::RIGHT_DOWN2,
			Direction::LEFT_DOWN2,
			// 長い利き
			Direction::RIGHT_UP,
			Direction::UP,
			Direction::LEFT_UP,
			Direction::RIGHT,
			Direction::LEFT,
			Direction::RIGHT_DOWN,
			Direction::DOWN,
			Direction::LEFT_DOWN,
			// 玉からの放射線上フラグ
			Direction::LEFT_DOWN,
			Direction::DOWN,
			Direction::RIGHT_DOWN,
			Direction::LEFT,
			Direction::RIGHT,
			Direction::LEFT_UP,
			Direction::UP,
			Direction::RIGHT_UP,
	};

	const unsigned int DirectionFlags::_dir2bit[] = {
			SHORT_RIGHT_UP2,   0,          SHORT_LEFT_UP2,   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			SHORT_RIGHT_UP,    SHORT_UP,   SHORT_LEFT_UP,    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			SHORT_RIGHT,       0,          SHORT_LEFT,       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			SHORT_RIGHT_DOWN,  SHORT_DOWN, SHORT_LEFT_DOWN,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			SHORT_RIGHT_DOWN2, 0,          SHORT_LEFT_DOWN2,
	};

	const unsigned int* DirectionFlags::dir2bit = DirectionFlags::_dir2bit + 33;
}

