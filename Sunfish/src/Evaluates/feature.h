/*
 * feature.h
 *
 *  Created on: 2012/07/07
 *      Author: ryosuke
 */

#ifndef FEATURE_H_
#define FEATURE_H_

#include <cassert>
#include "../Shogi/position.h"
#include "param.h"

namespace Evaluates {
	class Feature {
	private:
		static const int kppBlackHand[8];
		static const int kppWhiteHand[8];
		static const int kkpHand[8];

		static Value getKKP(const Param& p, const Kings& kings,
				const Shogi::Square& square,
				const Shogi::Piece& piece) {
			int squareIndex = square.getShortIndex();
			int pieceIndex = piece.getInteger();
			return Value(piece.isBlack() ?
				p.getKKP(kings, blackPiece[pieceIndex], squareIndex, true) :
				p.getKKP(kings, whitePiece[pieceIndex], 80 - squareIndex, false));
		}

		static Value getKKP_H(const Param& p, const Kings& kings, int baseIndex, int count, bool black) {
			return Value(black ? p.getKKP_H(kings, baseIndex, count, true) : p.getKKP_H(kings, baseIndex, count, false));
		}

		static void addKKP(Gradient& g, const Kings& kings,
				const Shogi::Square& square,
				const Shogi::Piece& piece, double inc) {
			int squareIndex = square.getShortIndex();
			int pieceIndex = piece.getInteger();
			if (piece.isBlack()) {
				g.addKKP(kings, blackPiece[pieceIndex], squareIndex, true, ValueF(inc));
			} else {
				g.addKKP(kings, whitePiece[pieceIndex], 80 - squareIndex, false, ValueF(inc));
			}
		}

		static void addKKP_H(Gradient& g, const Kings& kings, int baseIndex, int count, bool black, double inc) {
			if (black) {
				g.addKKP_H(kings, baseIndex, count, true, ValueF(inc));
			} else {
				g.addKKP_H(kings, baseIndex, count, false, ValueF(inc));
			}
		}

		static int getKPPIndex(const Shogi::Square& square,
				const Shogi::Piece& piece, bool black) {
			int squareIndex = square.getShortIndex();
			int pieceIndex = piece.getInteger();
			if (black) {
				return blackPiece[pieceIndex] + KPP_KNUM * squareIndex;
			} else {
				return whitePiece[pieceIndex] + KPP_KNUM * (80 - squareIndex);
			}
		}

		template<bool get, bool cum>
		static Value extract(const Shogi::Position& pos,
				const Param* pp, Gradient* pg, double inc);

		static Value getDiffKPP(const Param& p, int king,
				const int changeList[], int changeNum,
				const int list[], int num);

		static Value getDiffKPP(const Param& p, int king,
				int x, int y) {
			if (x < y) {
				return Value(p.getKPP(king, y, x));
			} else {
				return Value(p.getKPP(king, x, y));
			}
		}

		template <bool diff>
		static int listHand(int list0[], int list1[], int num,
				const Shogi::Position& pos,
				Shogi::Piece change = Shogi::Piece::EMPTY);

		Feature();

	public:
		static Value getValue(const Shogi::Position& pos,
				const Param* pparam) {
			return extract<true, false>(pos, pparam, NULL, 0.0);
		}

		static void incValue(const Shogi::Position& pos,
				Gradient* pg, double d) {
			extract<false, true>(pos, NULL, pg, d);
		}

		static Value getDiff(const Shogi::Position& pos,
				const Param& p, Shogi::Square from,
				Shogi::Square to, Shogi::Piece fromBefore,
				Shogi::Piece toBefore, Shogi::Piece toAfter);

		static Value estimate(const Shogi::Position& pos,
				const Param* pparam, const Shogi::Move& move);
	};
}

#endif // FEATURE_H_
