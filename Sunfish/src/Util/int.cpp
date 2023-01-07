/*
 * int.cpp
 *
 *  Created on: 2012/06/10
 *      Author: ryosuke
 */

#include <sstream>
#include <iomanip>
#include "int.h"

namespace Util {
	std::string Int::toString32(unsigned u32) {
		std::ostringstream oss;
		oss << std::setw(8) << std::setfill('0') << std::hex << u32;
		return oss.str();
	}
}
