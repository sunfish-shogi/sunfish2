/*
 * attackers.h
 *
 *  Created on: 2012/07/10
 *      Author: ryosuke
 */

#ifndef ATTACKERS_H_
#define ATTACKERS_H_

#include "../Shogi/position.h"
#include "../Evaluates/param.h"

namespace Search {
	class Attackers {
	private:
		static const int SIZE = 32;
		const Evaluates::Param& param;
		int blackNum;
		Evaluates::Value blackAttackers[SIZE];
		int whiteNum;
		Evaluates::Value whiteAttackers[SIZE];
		bool blackTurn;
		Evaluates::Value firstValue;
		Evaluates::Value defaultValue;

		template <bool black>
		void create(const Shogi::Position& pos, Shogi::Move move);

		void sort(Evaluates::Value attackers[], int num);

		template <bool black>
		Evaluates::Value see(int b, int w, Evaluates::Value v) const;

	public:
		Attackers(const Evaluates::Param& param) :
				param(param) {
		}

		Attackers(const Evaluates::Param& param,
				const Shogi::Position& pos, Shogi::Move move) :
				param(param) {
			create(pos, move);
		}

		void create(const Shogi::Position& pos, Shogi::Move move) {
			blackTurn = pos.isBlackTurn();
			firstValue = param.getPieceExchangeAbs(move.getPiece());
			Shogi::Piece piece = pos.getBoard(move.getTo());
			if (piece.isEmpty()) {
				defaultValue = Evaluates::Value(0);
			} else {
				defaultValue = param.getPieceExchangeAbs(piece);
			}
			create<true>(pos, move);
			create<false>(pos, move);
		}

		Evaluates::Value see() const {
			if (blackTurn) {
				return defaultValue - see<false>(0, 0, firstValue);
			} else {
				return defaultValue - see<true>(0, 0, firstValue);
			}
		}
	};
}

#endif // ATTACKERS_H_
