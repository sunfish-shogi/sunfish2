/*
 * squareDiff.cpp
 *
 *  Created on: 2012/05/30
 *      Author: ryosuke
 */

#include "squareDiff.h"

namespace Shogi {
	const int SquareDiff::_diff2dir[] = {
		-17,  0,  0,  0,  0,  0,  0,  0,-16,  0,  0,  0,  0,  0,  0,  0,
		-15,-17,  0,  0,  0,  0,  0,  0,-16,  0,  0,  0,  0,  0,  0,-15,
		  0,  0,-17,  0,  0,  0,  0,  0,-16,  0,  0,  0,  0,  0,-15,  0,
		  0,  0,  0,-17,  0,  0,  0,  0,-16,  0,  0,  0,  0,-15,  0,  0,
		  0,  0,  0,  0,-17,  0,  0,  0,-16,  0,  0,  0,-15,  0,  0,  0,
		  0,  0,  0,  0,  0,-17,  0,  0,-16,  0,  0,-15,  0,  0,  0,  0,
		  0,  0,  0,  0,  0,  0,-17,-33,-16,-31,-15,  0,  0,  0,  0,  0,
		  0,  0,  0,  0,  0,  0,  0,-17,-16,-15,  0,  0,  0,  0,  0,  0,
		 -1, -1, -1, -1, -1, -1, -1, -1,  0,  1,  1,  1,  1,  1,  1,  1,
		  1,  0,  0,  0,  0,  0,  0, 15, 16, 17,  0,  0,  0,  0,  0,  0,
		  0,  0,  0,  0,  0,  0, 15, 31, 16, 33, 17,  0,  0,  0,  0,  0,
		  0,  0,  0,  0,  0, 15,  0,  0, 16,  0,  0, 17,  0,  0,  0,  0,
		  0,  0,  0,  0, 15,  0,  0,  0, 16,  0,  0,  0, 17,  0,  0,  0,
		  0,  0,  0, 15,  0,  0,  0,  0, 16,  0,  0,  0,  0, 17,  0,  0,
		  0,  0, 15,  0,  0,  0,  0,  0, 16,  0,  0,  0,  0,  0, 17,  0,
		  0, 15,  0,  0,  0,  0,  0,  0, 16,  0,  0,  0,  0,  0,  0, 17,
		 15,  0,  0,  0,  0,  0,  0,  0, 16,  0,  0,  0,  0,  0,  0,  0,
		 17,
	};

	const int* SquareDiff::diff2dir = _diff2dir + 0x88;

	const int SquareDiff::_diff2dirOne[] = {
		  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
		  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
		  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
		  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
		  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
		  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
		  0,  0,  0,  0,  0,  0,  0,-33,  0,-31,  0,  0,  0,  0,  0,  0,
		  0,  0,  0,  0,  0,  0,  0,-17,-16,-15,  0,  0,  0,  0,  0,  0,
		  0,  0,  0,  0,  0,  0,  0, -1,  0,  1,  0,  0,  0,  0,  0,  0,
		  1,  0,  0,  0,  0,  0,  0, 15, 16, 17,  0,  0,  0,  0,  0,  0,
		  0,  0,  0,  0,  0,  0,  0, 31,  0, 33,  0,  0,  0,  0,  0,  0,
		  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
		  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
		  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
		  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
		  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
		  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
		  0,
	};

	const int* SquareDiff::diff2dirOne = _diff2dirOne + 0x88;

	const int SquareDiff::_diff2dis[] = {
		  8,  0,  0,  0,  0,  0,  0,  0,  8,  0,  0,  0,  0,  0,  0,  0,
		  8,  7,  0,  0,  0,  0,  0,  0,  7,  0,  0,  0,  0,  0,  0,  7,
		  0,  0,  6,  0,  0,  0,  0,  0,  6,  0,  0,  0,  0,  0,  6,  0,
		  0,  0,  0,  5,  0,  0,  0,  0,  5,  0,  0,  0,  0,  5,  0,  0,
		  0,  0,  0,  0,  4,  0,  0,  0,  4,  0,  0,  0,  4,  0,  0,  0,
		  0,  0,  0,  0,  0,  3,  0,  0,  3,  0,  0,  3,  0,  0,  0,  0,
		  0,  0,  0,  0,  0,  0,  2,  1,  2,  1,  2,  0,  0,  0,  0,  0,
		  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  0,  0,  0,  0,  0,  0,
		  8,  7,  6,  5,  4,  3,  2,  1,  0,  1,  2,  3,  4,  5,  6,  7,
		  8,  0,  0,  0,  0,  0,  0,  1,  1,  1,  0,  0,  0,  0,  0,  0,
		  0,  0,  0,  0,  0,  0,  2,  1,  2,  1,  2,  0,  0,  0,  0,  0,
		  0,  0,  0,  0,  0,  3,  0,  0,  3,  0,  0,  3,  0,  0,  0,  0,
		  0,  0,  0,  0,  4,  0,  0,  0,  4,  0,  0,  0,  4,  0,  0,  0,
		  0,  0,  0,  5,  0,  0,  0,  0,  5,  0,  0,  0,  0,  5,  0,  0,
		  0,  0,  6,  0,  0,  0,  0,  0,  6,  0,  0,  0,  0,  0,  6,  0,
		  0,  7,  0,  0,  0,  0,  0,  0,  7,  0,  0,  0,  0,  0,  0,  7,
		  8,  0,  0,  0,  0,  0,  0,  0,  8,  0,  0,  0,  0,  0,  0,  0,
		  8,
	};

	const int* SquareDiff::diff2dis = _diff2dis + 0x88;
}