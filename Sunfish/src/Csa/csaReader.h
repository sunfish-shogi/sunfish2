/*
 * csaReader.h
 * 
 *  Created on: 2012/06/09
 *      Author: Kubo Ryosuke
 */

#ifndef CSA_READER_H_
#define CSA_READER_H_

#include <iostream>
#include "csa.h"
#include "../Records/record.h"

namespace Csa {
	class CsaReader {
		private:
			static const int LINE_BUFFER_SIZE = 1024;
			CsaReader() {
			}

		public:
			enum LineStat {
				LINE_ERROR,
				LINE_EMPTY,
				LINE_TURN,
				LINE_BOARD,
				LINE_HAND,
				LINE_MOVE,
				LINE_VAR,
				LINE_INFO,
				LINE_TIME,
				LINE_STAT,
			};

			static bool parseLineBoard(const char* line, unsigned rank, Shogi::Position& pos);
			static bool parseLineHand(const char* line, Shogi::Position& pos, bool black);
			static bool parseLineMove(const char* line, const Shogi::Position& pos, Shogi::Move& move);
			static bool parseLineMove(const char* line, Records::Record& record);
			static LineStat parseLine(const char* line, Shogi::Position& pos);
			static LineStat parseLine(const char* line, Records::Record& record);
			static bool read(std::istream& in, Shogi::Position& pos);
			static bool read(const char* filename, Shogi::Position& pos);
			static bool read(const char* filename, Records::Record& record);
	};
}

#endif // CSA_READER_H_
