/*
 * PawnFlags.h
 *
 *  Created on: 2012/05/26
 *      Author: ryosuke
 */

#ifndef PAWNFLAGS_H_
#define PAWNFLAGS_H_

#include <string>

namespace Shogi{
	class PawnFlags{
	private:
		unsigned bits;

	public:
		PawnFlags() {
			setAll(0U);
		}

		PawnFlags(int file) {
			setAll(1 << file);
		}

		PawnFlags(const PawnFlags& pawnFlags) : bits(pawnFlags.bits) {
		}

		bool exist(int file) const {
			return (bits & (1 << file)) != 0U;
		}

		bool notExist(int file) const {
			return !exist(file);
		}

		void clear() {
			setAll(0U);
		}

		void setAll(int file) {
			bits = (1 << file);
		}

		void set(int file) {
			bits |= (1 << file);
		}

		void unset(int file) {
			bits &= ~(1 << file);
		}

		unsigned getBits() const {
			return bits;
		}

		void setBits(unsigned bits) {
			this->bits = bits;
		}

		std::string toString() const;
	};
}

#endif /* PAWNFLAGS_H_ */
