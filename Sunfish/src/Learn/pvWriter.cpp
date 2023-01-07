/*
 * pvWriter.cpp
 *
 *  Created on: 2013/02/07
 *      Author: ryosuke
 */

#include "pvIoFlag.h"
#include "pvWriter.h"

namespace Learns {
	using namespace Shogi;
	using namespace Search;

	bool PvWriter::writePosition(const Position& pos) {
		PvIoFlag(PvIoFlag::POSITION).writeBinary(fout);
		pos.writeBinary(fout);
		return !fout.fail();
	}

	bool PvWriter::writeMove(const Move& move) {
		PvIoFlag(PvIoFlag::MOVE).writeBinary(fout);
		move.writeBinary(fout);
		return !fout.fail();
	}

	bool PvWriter::writePv(const Pv& pv) {
		PvIoFlag(PvIoFlag::PV).writeBinary(fout);
		pv.writeBinary(fout);
		return !fout.fail();
	}
}
