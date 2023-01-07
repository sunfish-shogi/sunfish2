/*
 * record.h
 *
 *  Created on: 2012/06/16
 *      Author: ryosuke
 */

#ifndef RECORD_H_
#define RECORD_H_

#include "hashStack.h"
#include "../Shogi/position.h"

namespace Records {
	class Record {
	private:
		static const int STACK_SIZE = 1024;
		Shogi::Position pos;
		int num;
		int cur;
		Shogi::Move moveStack[STACK_SIZE];
		Shogi::Change changeStack[STACK_SIZE];
		HashData hashStack[STACK_SIZE];

	public:
		Record() {
			initStack();
		}

		Record(const Shogi::Handicap& handicap) : pos(handicap) {
			initStack();
		}

		Record(const Shogi::Position& pos) : pos(pos) {
			initStack();
		}

		void setHashStack(int index, const Shogi::Position& pos) {
			hashStack[index].positionHash = pos.getHash();
			hashStack[index].boardHash = pos.getBoardHash();
			hashStack[index].handHash = pos.getHandHash();
			hashStack[index].turnHash = pos.getTurnHash();
			hashStack[index].handSet = Shek::HandSet(pos.isBlackTurn()
					? pos.getBlackHand() : pos.getBlackHand());
			hashStack[index].blackTurn = pos.isBlackTurn();
			hashStack[index].check = pos.isCheck();
		}

		void initStack() {
			num = 0;
			cur = 0;
			setHashStack(0, pos);
		}

		void init(const Shogi::Position& pos) {
			initStack();
			this->pos.copy(pos);
		}

		bool move(const Shogi::Move& move) {
			Shogi::Change change;
			if (cur < STACK_SIZE && pos.move(move, change)) {
				moveStack[cur] = move;
				changeStack[cur] = change;
				num = ++cur;
				setHashStack(cur, pos);
				return true;
			}
			return false;
		}

		bool next() {
			if (cur < num) {
				pos.move(moveStack[cur]);
				cur++;
				return true;
			}
			return false;
		}

		bool prev() {
			if (cur > 0) {
				cur--;
				pos.back(changeStack[cur]);
				return true;
			}
			return false;
		}

		void begin() {
			while (prev())
				;
		}

		void end() {
			while (next())
				;
		}

		bool to(int index) {
			while (cur > index && prev())
				;
			while (cur < index && next())
				;
			return cur == index;
		}

		const Shogi::Position& getPosition() const {
			return pos;
		}

		int getSize() const {
			return num;
		}

		int getCurrent() const {
			return cur;
		}

		bool hasNext() const {
			return cur < num;
		}

		bool getNextMove(Shogi::Move& move) const {
			if (cur < num) {
				move = moveStack[cur];
				return true;
			}
			return false;
		}

		bool getPrevMove(Shogi::Move& move) const {
			if (cur > 0) {
				move = moveStack[cur-1];
				return true;
			}
			return false;
		}

		Util::uint64 getHash(int index) {
			return hashStack[index].positionHash;
		}

		bool isRepetition(bool full = true) const;

		const HashStack getHashStack() const {
			return HashStack(hashStack, cur);
		}

		std::string toString() const {
			return pos.toString();
		}
	};
}

#endif // RECORD_H_
