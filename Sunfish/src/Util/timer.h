/*
 * timer.h
 *
 *  Created on: 2013/02/24
 *      Author: ryosuke
 */

#ifndef TIMER_H_
#define TIMER_H_

#ifdef WIN32
#	include <windows.h>
#endif
#include <ctime>

namespace Util {
	class Timer{
	private:
#ifdef WIN32
		LARGE_INTEGER time_b;
#else
		struct timespec time_b;
#endif

	public:
		void set() {
#ifdef WIN32
			QueryPerformanceCounter(&time_b);
#elif defined(CLOCK_MONOTONIC_RAW)
			clock_gettime(CLOCK_MONOTONIC_RAW, &time_b);
#else
			clock_gettime(CLOCK_MONOTONIC, &time_b);
#endif
		}

		double get() const {
#ifdef WIN32
			LARGE_INTEGER time_n, freq;
			QueryPerformanceCounter(&time_n);
			QueryPerformanceFrequency(&freq);
			return (time_n.QuadPart - time_b.QuadPart + 1) / (double)freq.QuadPart;
#else
			struct timespec time_n;
#	if defined(CLOCK_MONOTONIC_RAW)
			clock_gettime(CLOCK_MONOTONIC_RAW, &time_n);
#	else
			clock_gettime(CLOCK_MONOTONIC, &time_n);
#	endif
			return (time_n.tv_sec - time_b.tv_sec)
				+ (time_n.tv_nsec - time_b.tv_nsec) * 1.0e-9;
#endif
		}
	};
}

#endif // TIMER_H_
