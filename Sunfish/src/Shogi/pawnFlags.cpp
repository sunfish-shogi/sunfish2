/*
 * pawnFlags.cpp
 *
 *  Created on: 2012/05/26
 *      Author: ryosuke
 */

#include <sstream>
#include "pawnFlags.h"
#include "square.h"

namespace Shogi{
	std::string PawnFlags::toString() const {
		std::ostringstream oss;
		for (int file = Square::FILE_NUM; file >= 1; file--) {
			oss << (exist(file) ? " x " : "   ");
		}
		return oss.str();
	}
};
