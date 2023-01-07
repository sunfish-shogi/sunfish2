/*
 * csaWriter.cpp
 * 
 *  Created on: 2012/07/31
 *      Author: Kubo Ryosuke
 */

#include <fstream>
#include "csaWriter.h"
#include "../Log/logger.h"

namespace Csa {
		using namespace Shogi;

		bool CsaWriter::write(std::ostream& out, const Position& pos) {
			out << pos.toStringCsa();
			return out.good();
		}

		bool CsaWriter::write(const char* filename, const Position& pos) {
			std::ofstream fout(filename);
			if (!fout) {
				Log::error.fileOpenError(filename);
				return false;
			}
			if (!write(fout, pos)) {
				fout.close();
				return false;
			}
			fout.close();
			return true;
		}

		bool CsaWriter::write(const char* filename, Records::Record& record) {
			std::ofstream fout(filename);
			if (!fout) {
				Log::error.fileOpenError(filename);
				return false;
			}
			int index = record.getCurrent();
			record.begin();
			if (!write(fout, record.getPosition())) {
				fout.close();
				return false;
			}
			while (record.next()) {
				Move move;
				if (record.getPrevMove(move)) {
					fout << move.toStringCsa() << '\n';
				}
			}
			record.to(index);
			fout.close();
			return true;
		}
}
