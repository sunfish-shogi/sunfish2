/*
 * pvReader.cpp
 *
 *  Created on: 2013/02/07
 *      Author: ryosuke
 */

#include "pvIoFlag.h"
#include "pvReader.h"

namespace Learns {
	using namespace Shogi;
	using namespace Search;

	bool PvReader::readPosition(Position& pos) {
		if (!nextFlag().is(PvIoFlag::POSITION)) {
			return false;
		}
		pos.readBinary(fin);
		next = PvIoFlag::NON;
		return !fin.fail();
	}

	bool PvReader::readMove(Move& move) {
		if (!nextFlag().is(PvIoFlag::MOVE)) {
			return false;
		}
		move.readBinary(fin);
		next = PvIoFlag::NON;
		return !fin.fail();
	}

	bool PvReader::readPv(Pv& pv) {
		if (!nextFlag().is(PvIoFlag::PV)) {
			return false;
		}
		pv.readBinary(fin);
		next = PvIoFlag::NON;
		return !fin.fail();
	}
}
