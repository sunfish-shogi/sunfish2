/*
 * pvWriter.h
 *
 *  Created on: 2013/02/07
 *      Author: ryosuke
 */

#ifndef PVWRITER_H_
#define PVWRITER_H_

#include "pvIoFlag.h"
#include "../Shogi/position.h"
#include "../Search/pv.h"
#include <fstream>

namespace Learns {
	class PvWriter {
	private:
		std::ofstream fout;

	public:
		PvWriter(const char* fname) {
			fout.open(fname, std::ios::out | std::ios::binary);
		}

		~PvWriter() {
			fout.close();
		}

		bool good() {
			return fout.good();
		}

		bool fail() {
			return fout.fail();
		}

		bool bad() {
			return fout.bad();
		}

		bool writePosition(const Shogi::Position& pos);

		bool writeMove(const Shogi::Move& move);

		bool writePv(const Search::Pv& pv);
	};
}

#endif // PVWRITER_H_
