/*
 * hash.cpp
 *
 *  Created on: 2013/01/13
 *      Author: ryosuke
 */

#include <fstream>
#include <iostream>
#include <boost/random.hpp>
#include "hash.h"
#include "../Log/logger.h"

namespace Util {
	const char* Hash::DEFAULT_FILE_NAME = "hash";

	void Hash::init() {
		boost::mt19937 rgen(static_cast<unsigned>(time(NULL)));
		boost::uniform_int<> dst(0, 0xffff);
		boost::variate_generator<boost::mt19937&, boost::uniform_int<> > r(rgen, dst);

		for (unsigned i = 0; i < size; i++) {
			values[i] = ((Util::uint64)r() << 48) ^ ((Util::uint64)r() << 32)
					^ ((Util::uint64)r() << 16) ^ (Util::uint64)r();
		}
	}

	bool Hash::read(const char* filename) {
		std::ifstream fin(filename, std::ios::in | std::ios::binary);
		if (!fin) {
			Log::error.fileOpenError(filename);
			return false;
		}
		fin.read((char*)values, sizeof(Util::uint64) * size);
		if (fin.fail()) {
			Log::error.fileIoError(filename);
			return false;
		}
		fin.close();
		return true;
	}

	bool Hash::write(const char* filename) {
		std::ofstream fout(filename, std::ios::out | std::ios::binary);
		if (!fout) {
			Log::error.fileOpenError(filename);
			return false;
		}
		fout.write((char*)values, sizeof(Util::uint64) * size);
		if (fout.fail()) {
			Log::error.fileIoError(filename);
			return false;
		}
		fout.close();
		return true;
	}
}
