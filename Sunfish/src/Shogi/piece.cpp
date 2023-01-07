/*
 * piece.cpp
 *
 *  Created on: 2012/05/12
 *      Author: ryosuke
 */

#include "piece.h"
#include "../Csa/csa.h"

namespace Shogi {
	const char Piece::pieceName[][4] = {
			"* ",
			"FU", "KY", "KE", "GI", "KI", "KA", "HI", "OU",
			"TO", "NY", "NK", "NG", "??", "UM", "RY", "??",
	};

	const char Piece::pieceNameS[][4] = {
			"* ",
			"fu", "ky", "ke", "gi", "ki", "ka", "hi", "ou",
			"to", "ny", "nk", "ng", "??", "um", "ry", "??",
	};

	const char Piece::UNKNOWN_NAME[] = "??";

	const DirectionFlags Piece::direction[] = {
			DirectionFlags(DirectionFlags::NON),

			DirectionFlags(DirectionFlags::BPIECE),
			DirectionFlags(DirectionFlags::BLANCE),
			DirectionFlags(DirectionFlags::BKNIGHT),
			DirectionFlags(DirectionFlags::BSILVER),
			DirectionFlags(DirectionFlags::BGOLD),
			DirectionFlags(DirectionFlags::BBISHOP),
			DirectionFlags(DirectionFlags::BROOK),
			DirectionFlags(DirectionFlags::BKING),
			DirectionFlags(DirectionFlags::BGOLD),
			DirectionFlags(DirectionFlags::BGOLD),
			DirectionFlags(DirectionFlags::BGOLD),
			DirectionFlags(DirectionFlags::BGOLD),
			DirectionFlags(DirectionFlags::NON),
			DirectionFlags(DirectionFlags::BHORSE),
			DirectionFlags(DirectionFlags::BDRAGON),

			DirectionFlags(DirectionFlags::NON),

			DirectionFlags(DirectionFlags::WPIECE),
			DirectionFlags(DirectionFlags::WLANCE),
			DirectionFlags(DirectionFlags::WKNIGHT),
			DirectionFlags(DirectionFlags::WSILVER),
			DirectionFlags(DirectionFlags::WGOLD),
			DirectionFlags(DirectionFlags::WBISHOP),
			DirectionFlags(DirectionFlags::WROOK),
			DirectionFlags(DirectionFlags::WKING),
			DirectionFlags(DirectionFlags::WGOLD),
			DirectionFlags(DirectionFlags::WGOLD),
			DirectionFlags(DirectionFlags::WGOLD),
			DirectionFlags(DirectionFlags::WGOLD),
			DirectionFlags(DirectionFlags::NON),
			DirectionFlags(DirectionFlags::WHORSE),
			DirectionFlags(DirectionFlags::WDRAGON),
	};

	Piece Piece::parseCsa(const char* str) {
		Piece piece(EMPTY);
		for (; str[0] == ' '; str++)
			;
		if (str[0] == Csa::CHAR_BLK) {
			piece.turnBlack();
			str++;
		} else if (str[0] == Csa::CHAR_WHT) {
			piece.turnWhite();
			str++;
		}
		for (unsigned i = PAWN; i <= DRAGON; i++) {
			if ((str[0] == pieceName[i][0] && str[1] == pieceName[i][1]) ||
					(str[0] == pieceNameS[i][0] && str[1] == pieceNameS[i][1])) {
				piece.piece |= i;
				return piece;
			}
		}
		return Piece(EMPTY);
	}
}
