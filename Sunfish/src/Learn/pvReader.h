/*
 * pvReader.h
 *
 *  Created on: 2013/02/07
 *      Author: ryosuke
 */

#ifndef PVREADER_H_
#define PVREADER_H_

#include "pvIoFlag.h"
#include "../Shogi/position.h"
#include "../Search/pv.h"
#include <fstream>

namespace Learns {
	class PvReader {
	private:
		std::ifstream fin;
		PvIoFlag next;

	public:
		PvReader(const char* fname) {
			fin.open(fname, std::ios::out | std::ios::binary);
		}

		~PvReader() {
			fin.close();
		}

		bool good() const {
			return fin.good();
		}

		bool fail() const {
			return fin.fail();
		}

		bool bad() const {
			return fin.bad();
		}

		PvIoFlag nextFlag() {
			if (next.is(PvIoFlag::NON)) {
				next.readBinary(fin);
				if (fin.eof()) {
					next = PvIoFlag::END;
				}
			}
			return next;
		}

		bool readPosition(Shogi::Position& pos);

		bool readMove(Shogi::Move& move);

		bool readPv(Search::Pv& pv);
	};
}

#endif // PVREADER_H_
