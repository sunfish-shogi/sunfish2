/*
 * int.h
 *
 *  Created on: 2012/06/10
 *      Author: ryosuke
 */

#ifndef INT64_H_
#define INT64_H_

#include <string>

namespace Util {
#ifdef _MSC_VER
	typedef unsigned __int64 uint64;
	typedef __int64 int64;
#	define ULLONG_MAX		_UI64_MAX
#	define LLONG_MAX		_I64_MAX
#	define U64(a)					( (uint64)a )
#else
	typedef unsigned long long uint64;
	typedef long long int64;
#	define U64(a)					( a ## ULL )
#endif

	class Int {
	private:
		Int() {}

	public:
		static std::string toString32(unsigned u32);

		static std::string toString64(uint64 u64) {
			return toString32((unsigned)(u64>>32))
				+ toString32((unsigned)u64);
		}
	};
}

#endif // INT64_H_
