/*
 * effectBoard.h
 *
 *  Created on: 2012/05/15
 *      Author: ryosuke
 */

#ifndef EFFECTBOARD_H_
#define EFFECTBOARD_H_

#include <cstring>
#include "board.h"
#include "square.h"
#include "directionFlags.h"

namespace Shogi {
	class EffectBoard {
	private:
		DirectionFlags blackEffectBoard[Square::SIZE];
		DirectionFlags whiteEffectBoard[Square::SIZE];

		template <bool black>
		DirectionFlags* effectBoard() {
			return black ? blackEffectBoard : whiteEffectBoard;
		}

		const DirectionFlags* effectBoardConst(bool black) const {
			return black ? blackEffectBoard : whiteEffectBoard;
		}

		template <bool black>
		void createEffect(const Board& board, const Square& sq);

		template <bool black, bool addition>
		void changeStraight(const Square& sq, const Direction dir, const DirectionFlags& dirFlags, const Board& board);

		template <bool black, bool addition>
		void changeAround(const Square& sq, const Board& board);

	public:
		EffectBoard() {
			init();
		}

		EffectBoard(const Board& board) {
			init(board);
		}

		EffectBoard(const EffectBoard& effectBoard) {
			init(effectBoard);
		}

		void init();

		void init(const Board& board);

		void init(const EffectBoard& effectBoard) {
#if 0
			for (unsigned i = 0; i < Square::SIZE; i++) {
				blackEffectBoard[i] = effectBoard.blackEffectBoard[i];
				whiteEffectBoard[i] = effectBoard.whiteEffectBoard[i];
			}
#else
			memcpy(blackEffectBoard, effectBoard.blackEffectBoard, sizeof(blackEffectBoard));
			memcpy(whiteEffectBoard, effectBoard.whiteEffectBoard, sizeof(whiteEffectBoard));
#endif
		}

		template <bool black>
		void set(const Square& sq, const DirectionFlags& effect) {
			effectBoard<black>()[sq.getIndex()].set(effect);
		}

		template <bool black>
		void add(const Square& sq, const DirectionFlags& effect) {
			effectBoard<black>()[sq.getIndex()].add(effect);
		}

		template <bool black>
		void remove(const Square& sq, const DirectionFlags& effect) {
			effectBoard<black>()[sq.getIndex()].remove(effect);
		}

		template <bool black>
		bool isWall(const Square& sq) const {
			return effectBoard<black>()[sq.getIndex()].isWall();
		}

		const DirectionFlags& get(const Square& sq, bool black) const {
			return effectBoardConst(black)[sq.getIndex()];
		}

		template <bool black, bool addition>
		void change(const Square& sq, const DirectionFlags& dirFlags, const Board& board);

		bool equals(const EffectBoard& effectBoard) const;

		std::string toString(bool king = false) const;
	};
}

#endif /* EFFECTBOARD_H_ */
