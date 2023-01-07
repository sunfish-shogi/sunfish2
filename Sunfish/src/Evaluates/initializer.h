/*
 * initializer.h
 *
 * Created on: 2012/07/01
 *     Author: ryosuke
 */

#ifndef INITIALIZER_H_
#define INITIALIZER_H_

#include "param.h"

namespace Evaluates {
	class Initializer {
	private:
		static const int pieceValues[];

		Initializer() {
		}

	public:
		static void apply(Param& param);

		static void apply(Gradient& gradient);
	};
}

#endif // INITIALIZER_H_
