/*
 * bit.h
 *
 *  Created on: 2012/05/27
 *      Author: ryosuke
 */

#ifndef BIT_H_
#define BIT_H_

namespace Util {
	class Bit {
	private:
		Bit() {
		}

	public:
		static unsigned count(unsigned x) {
			x = x - ( ( x >> 1 ) & 0x55555555U );
			x = ( x & 0x33333333U ) + ( ( x >> 2) & 0x33333333U );
			x = ( x + ( x >> 4 ) ) & 0x0F0F0F0FU;
			x = x + ( x >> 8 );
			x = x + ( x >> 16 );
			return x & 0x0000003FU;
		}
	};
}

#endif /* BIT_H_ */
