/*
 * pvHandler.h
 *
 *  Created on: 2012/07/10
 *      Author: ryosuke
 */

#ifndef PVHANDLER_H_
#define PVHANDLER_H_

#include "pv.h"

namespace Search {
	class PvHandler {
	private:

	public:
		virtual void pvHandler(const Pv& pv,
				Evaluates::Value value,
				Util::uint64 nodes, int depth,
				double seconds) = 0;

		virtual void failHigh(const Pv& pv,
				Evaluates::Value value,
				Util::uint64 nodes, int depth,
				double seconds) = 0;

		virtual void failLow(Evaluates::Value value,
				Util::uint64 nodes, int depth, double seconds) = 0;
	};
}

#endif // PVHANDLER_H_
