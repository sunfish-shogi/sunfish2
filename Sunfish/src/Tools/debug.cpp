/*
 * debug.cpp
 *
 *  Created on: 2012/05/12
 *      Author: ryosuke
 */


#ifndef NDEBUG

#include "debug.h"

namespace Tools{
	using namespace Shogi;

	bool Debug::positionOk(const Position pos) {
		// hash
		Util::uint64 hash = pos.generateBoardHash() ^ pos.generateHandHash() ^ pos.getTurnHash();
		if (pos.getHash() != hash) {
			std::cout << "***** HASH CODE ERROR!!! *****\n";
			std::cout << std::hex << hash << '\n';
			DEBUG_PRINT_LINE;
			std::cout.flush();
			return false;
		}
		// effect
		Position temp(pos);
		temp.update();
		if (!temp.getEffectBoard().equals(pos.getEffectBoard())) {
			std::cout << "***** EFFECT BOARD ERROR!!! *****\n";
			std::cout << temp.toStringEffect(true);
			DEBUG_PRINT_LINE;
			std::cout.flush();
			return false;;
		}
		return true;
	}

	bool Debug::positionError(const Position& pos) {
		for (Square square = Square::TOP; square.valid(); square.next()) {
			Piece piece = pos.getBoard(square);
			if (piece.isWall()) {
				return true;
			}
		}
		return false;
	}
}

#endif // NDEBUG
