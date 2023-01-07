/*
 * csaWriter.h
 * 
 *  Created on: 2012/07/31
 *      Author: Kubo Ryosuke
 */

#include <iostream>
#include "../Records/record.h"

namespace Csa {
	class CsaWriter {
	private:
		static bool write(std::ostream& out, const Shogi::Position& pos);

	public:
		static bool write(const char* filename, const Shogi::Position& pos);
		static bool write(const char* filename, Records::Record& record);
	};
}
