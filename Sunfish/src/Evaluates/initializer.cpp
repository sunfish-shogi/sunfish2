/*
 * initializer.cpp
 *
 * Created on: 2012/07/01
 *     Author: ryosuke
 */

#include "initializer.h"

namespace Evaluates {
	using namespace Shogi;

	const int Initializer::pieceValues[] = {
		   0, // empty
		 100, // pawn
		 300, // lance
		 300, // knight
		 400, // silver
		 500, // gold
		 600, // bishop
		 700, // rook
		   0, // king
		 500, // tokin
		 500, // promoted lance
		 500, // promoted knight
		 500, // promoted silver
		   0, // N/a
		 900, // horse
		1000, // dragon
	};

	void Initializer::apply(Param& param) {
		param.init();
		for (Piece p = Piece::PAWN; p <= Piece::DRAGON; p.toNext()) {
			param.setPiece(p, pieceValues[p]);
		}
		param.cumulate();
	}

	void Initializer::apply(Gradient& gradient) {
		gradient.init();
	}
}
