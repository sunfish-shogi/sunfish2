/*
 * remainingTime.h
 *
 *  Created on: 2013/03/23
 *      Author: ryosuke
 */

#ifndef REMAININGTIME_H_
#define REMAININGTIME_H_

#include <string>
#include <sstream>

namespace Network {
	class RemainingTime {
	private:
		int total;
		int remain;
		int readoff;

	public:
		RemainingTime() {
		}

		RemainingTime(int total, int readoff)
				: total(total), readoff(readoff) {
			reset();
		}

		void init(int total, int readoff = 0) {
			this->total = total;
			this->readoff = readoff;
			reset();
		}

		void reset() {
			remain = total;
		}

		int use(int sec) {
			remain = remain > sec ? remain - sec : 0;
			return remain;
		}

		int usable() const {
			return remain + readoff;
		}

		int isLimited() const {
			return total != 0 || readoff != 0;
		}

		int getTotal() const {
			return total;
		}

		int getRemain() const {
			return remain;
		}

		int getReadoff() const {
			return readoff;
		}

		std::string toString() const {
			std::ostringstream oss;
			oss << remain << '/' << total << ' ' << readoff;
			return oss.str();
		}

	};
}

#endif // REMAININGTIME_H_
