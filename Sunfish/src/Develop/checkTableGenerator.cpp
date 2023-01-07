/*
 * checkTableGenerator.cpp
 *
 *  Created on: 2012/09/29
 *      Author: ryosuke
 */

#include "checkTableGenerator.h"
#include "../Shogi/squareDiff.h"
#include "../Shogi/attack.h"
#include <sstream>

namespace Develop {
	using namespace Shogi;

	std::string CheckTableGenerator::develop() {
		std::ostringstream oss;

		oss << "#include \"../attack.h\"\n";
		oss << "namespace Shogi {\n";

		oss << "const unsigned Attack::_checkTable[][17][17] =\n";
		oss << "{\n";
		for (Piece piece = Piece::BPAWN; piece <= Piece::WDRAGON; piece.toNext()) {
			// 玉の位置
			oss << "{";
			for (int rank1 = -8; rank1 <= 8; rank1++) {
				oss << "{";
				for (int file1 = -8; file1 <= 8; file1++) {
					DirectionFlags flags;
					if (piece != Piece::BKING && piece != Piece::WKING) {
						// 移動先
						for (int rank2 = -8; rank2 <= 8; rank2++) {
							for (int file2 = -8; file2 <= 8; file2++) {
								int rank = rank1 - rank2;
								int file = file1 - file2;
								if (rank >= -8 && rank <= 8 && file >= -8 && file <= 8 &&
										Attack::attack(piece, rank2, file2) != 0 && // 移動先に動けるか
										(Attack::attack(piece, rank, file) != 0 ||
										Attack::attack(piece.getPromoted(), rank, file) != 0)) { // 王手し得る位置か
									SquareDiff diff(file2+(rank2<<Square::SHIFT_RANK));
									Direction dir = diff.toDirection();
									if (piece.getMovableDirection().check(DirectionFlags(dir))) {
										flags.add(dir);
									} else if (piece.getMovableDirection().check(DirectionFlags(dir, true))) {
										flags.add(DirectionFlags(dir, true));
									} else {
										assert(false);
									}
								}
							}
						}
					}
					oss << std::hex;
					oss << "0x" << flags.getInteger() << ",";
					oss << std::dec;
				}
				oss << "},\n";
			}
			oss << "},\n";
		}
		oss << "};\n";

		oss << "const unsigned (*Attack::checkTable)[17][17] =\n";
		oss << "(unsigned (*)[17][17])&_checkTable[-1][8][8];\n";

		oss << "}\n"; // namespace

		return oss.str();
	}
}
