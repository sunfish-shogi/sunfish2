/*
 * shekTable.h
 *
 *  Created on: 2012/11/03
 *      Author: ryosuke
 */

#ifndef SHEKTABLE_H_
#define SHEKTABLE_H_

#include "shekEntity.h"
#include "../Shogi/position.h"
#include "../Records/hashStack.h"
#include "../Table/baseTable.h"

namespace Shek {
	class ShekTable : public Table::BaseTable<ShekEntities> {
	public:
		// TODO: サイズの指定を設定ファイルから読み込めるように
		ShekTable(unsigned bits = 18) : 
				Table::BaseTable<ShekEntities>(bits) {
		}

		void set(const Records::HashStack& hashStack);

		void unset(const Records::HashStack& hashStack);

		void set(const Shogi::Position& pos) {
			const Shogi::Hand& hand = pos.isBlackTurn()
					? pos.getBlackHand() : pos.getBlackHand();
			Util::uint64 hash = pos.getBoardHash();
			_getEntity(hash).set(hash,
					HandSet(hand), pos.isBlackTurn()
#ifndef NDEBUG
					, -1
#endif // NDEBUG
					);
		}

		void unset(const Shogi::Position& pos) {
			Util::uint64 hash = pos.getBoardHash();
			_getEntity(hash).unset(hash);
		}

		ShekStat check(const Shogi::Position& pos) const {
			const Shogi::Hand& hand = pos.isBlackTurn()
					? pos.getBlackHand() : pos.getBlackHand();
			Util::uint64 hash = pos.getBoardHash();
			return getEntity(hash).check(hash,
					HandSet(hand), pos.isBlackTurn());
		}

#ifndef NDEBUG
		void debugPrint(const Shogi::Position& pos) const {
			const Shogi::Hand& hand = pos.isBlackTurn()
					? pos.getBlackHand() : pos.getBlackHand();
			Util::uint64 hash = pos.getBoardHash();
			getEntity(hash).debugPrint(hash,
					HandSet(hand), pos.isBlackTurn());
		}
#endif
	};
}

#endif // SHEKTABLE_H_
