/*
 * debug.h
 *
 *  Created on: 2012/05/12
 *      Author: ryosuke
 */

#ifndef DEBUG_H_
#define DEBUG_H_

#include <iostream>
#include "../Shogi/position.h"

#ifdef NDEBUG
#define DEBUG_PRINT_LINE		do { } while (false)
#define DEBUG_PRINT(v)			do { } while (false)
#else
#define DEBUG_PRINT_LINE		do { std::cout << __FILE__ << "(" << __LINE__ << ")\n"; } while (false)
#define DEBUG_PRINT(v)			do { std::cout << __FILE__ << "(" << __LINE__ << ") " << #v << "=" << (v) << '\n'; } while (false)
#endif

namespace Tools{
	class Debug {
	private:
		Debug() {
		}

	public:
		static bool positionOk(const Shogi::Position pos);

		static bool positionError(const Shogi::Position& pos);
	};
}

#endif /* DEBUG_H_ */
