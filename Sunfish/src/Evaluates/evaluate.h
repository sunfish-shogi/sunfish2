/*
 * evaluate.h
 *
 *  Created on: 2012/07/01
 *      Author: ryosuke
 */

#ifndef EVALUATE_H_
#define EVALUATE_H_

#include "evaluateTable.h"
#include "feature.h"

namespace Evaluates {
	class Evaluate {
	private:
		const Param& param;
		Value baseValue;
		EvaluateTable table;

		Value posValue;

	public:
		Evaluate(const Param& param, Value baseValue = Value(0),
				Value posValue = Value(0)) :
				param(param), baseValue(baseValue),
				posValue(posValue) {
		}

		Evaluate(const Param& param, const Shogi::Position& pos) :
				param(param) {
			init(pos);
		}

		void init(const Shogi::Position& pos);

		void addBaseValue(const Shogi::Piece piece) {
			baseValue += param.getPiece(piece);
		}

		void subBaseValue(const Shogi::Piece piece) {
			baseValue -= param.getPiece(piece);
		}

		void addBaseValueEx(const Shogi::Piece piece) {
			baseValue += param.getPieceExchange(piece);
		}

		void subBaseValueEx(const Shogi::Piece piece) {
			baseValue -= param.getPieceExchange(piece);
		}

		void addBaseValuePr(const Shogi::Piece piece) {
			baseValue += param.getPiecePromote(piece);
		}

		void subBaseValuePr(const Shogi::Piece piece) {
			baseValue -= param.getPiecePromote(piece);
		}

		Value getBaseValue() const {
			return baseValue;
		}

		void setBaseValue(const Value& baseValue) {
			this->baseValue = baseValue;
		}

		// 駒を打った場合
		void updatePositionalValue(
				const Shogi::Position& pos,
				const Shogi::Square& to,
				const Shogi::Piece& toAfter) {
			posValue += Feature::getDiff(
					pos, param, Shogi::Square::NON, to,
					Shogi::Piece::EMPTY, Shogi::Piece::EMPTY,
					toAfter);
		}

		// 盤上の駒の移動の場合
		void updatePositionalValue(
				const Shogi::Position& pos,
				const Shogi::Square& from,
				const Shogi::Square& to,
				const Shogi::Piece& fromBefore,
				const Shogi::Piece& toBefore,
				const Shogi::Piece& toAfter) {
			Util::uint64 hash = pos.getHash();
			if (toAfter.isKing()) {
				assert(fromBefore == toAfter);
				if (!table.get(hash, posValue)) {
					posValue = Feature::getValue(pos, &param);
					table.set(hash, posValue);
				}
			} else {
				posValue += Feature::getDiff(
						pos, param, from, to,
						fromBefore, toBefore, toAfter);
				table.set(hash, posValue);
			}
		}

		Value getPositionalValue() const {
			return posValue;
		}

		void setPositionalValue(const Value& posValue) {
			this->posValue = posValue;
		}

#if 0
		Value _getPositionalValue(const Shogi::Position& pos) {
			Util::uint64 hash = pos.getHash();
			Value value;
			if (!table.get(hash, value)) {
				value = Feature::getValue(pos, &param);
				table.set(hash, value);
			}
			return value;
		}
#endif

		Value getValue() {
			return baseValue + getPositionalValue() / Param::SCALE;
		}

		const Param& getParam() const {
			return param;
		}

		Value estimate(const Shogi::Position& pos,
				const Shogi::Move move) const {
			Value value = Feature::estimate(pos, &param, move) / Param::SCALE;
			Shogi::Piece cap = pos.getBoard(move.getTo());
			if (!cap.isEmpty()) {
				value -= param.getPieceExchange(cap);
			}
			if (move.isPromotion()) {
				value += param.getPiecePromote(move.getPiece());
			}
			if (move.getPiece().isKing()) {
				value += 1100;
			} else {
				value += 280;
			}
			return value;
		}
	};
}

#endif // EVALUATE_H_
