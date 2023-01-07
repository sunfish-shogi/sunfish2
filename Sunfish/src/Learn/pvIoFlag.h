/*
 * pvIO.h
 *
 *  Created on: 2013/02/07
 *      Author: ryosuke
 */

#ifndef PVIOFLAG_H_
#define PVIOFLAG_H_

#include <iostream>

namespace Learns {
	class PvIoFlag {
	private:
		char flag;

	public:
		enum Flag {
			NON,
			POSITION,
			MOVE,
			PV,
			END,
		};

		explicit PvIoFlag(Flag flag = NON) {
			this->flag = (char)flag;
		}

		explicit PvIoFlag(std::istream& in) {
			readBinary(in);
		}

		const PvIoFlag& operator=(Flag flag) {
			this->flag = flag;
			return *this;
		}

		const PvIoFlag& operator=(const PvIoFlag f) {
			flag = f.flag;
			return *this;
		}

		bool is(Flag flag) const {
			return this->flag == flag;
		}

		Flag get() const {
			return (Flag)flag;
		}

		bool writeBinary(std::ostream& out) const {
			out.write(&flag, sizeof(flag));
			return !out.fail();
		}

		bool readBinary(std::istream& in) {
			in.read(&flag, sizeof(flag));
			return !in.fail();
		}
	};
}

#endif // PVIOFLAG_H_
