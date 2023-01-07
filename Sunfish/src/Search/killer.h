/*
 * killer.h
 *
 *  Created on: 2013/01/06
 *      Author: ryosuke
 */

#ifndef KILLER_H_
#define KILLER_H_

#include "../Evaluates/param.h"
#include "../Shogi/move.h"

namespace Search {
	class Killer {
	private:
		Shogi::Move killer1;
		Shogi::Move killer2;
		Evaluates::Value value1;
		Evaluates::Value value2;

	public:
		Killer() {
		}

		Killer(const Killer& source) {
			killer1 = source.killer1;
			killer2 = source.killer2;
			value1 = source.value1;
			value2 = source.value2;
		}

		void init() {
			killer1.setEmpty();
			killer2.setEmpty();
		}

		void add(const Shogi::Move& move, const Evaluates::Value& value) {
			if (move == killer1) {
				value1 = value;
			} else if (value > value1) {
				killer2 = killer1;
				value2 = value1;
				killer1 = move;
				value1 = value;
			} else if (move == killer2) {
				value2 = value;
			} else if (value > value2) {
				killer2 = move;
				value2 = value;
			}
		}

		const Shogi::Move& get1() const {
			return killer1;
		}

		const Shogi::Move& get2() const {
			return killer2;
		}

		const Evaluates::Value& getValue1() const {
			return value1;
		}

		const Evaluates::Value& getValue2() const {
			return value2;
		}

		static Evaluates::Value calcCurrentChange(
				const Shogi::Position& pos,
				const Shogi::Move& move,
				const Evaluates::Param& param) {
			Shogi::Piece cap = pos.getBoard(move.getTo());
			Evaluates::Value value = Evaluates::Value(param.getPieceExchange(cap));
			if (move.isPromotion()) {
				value += param.getPiecePromote(move.getPiece());
			}
			return value;
		}
	};
}

#endif // KILLER_H_
