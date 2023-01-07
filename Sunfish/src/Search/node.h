/*
 * node.h
 *
 *  Created on: 2012/06/16
 *      Author: ryosuke
 */

#ifndef NODE_H_
#define NODE_H_

#include "pv.h"
#include "phasedMoveGenerator.h"
#include "history.h"

namespace Search {
	class Node {
	private:
		PhasedMoveGenerator* pgen;
		const Shogi::Move* pmove;
		Shogi::Change change;
		Evaluates::Value baseValue;
		Evaluates::Value posValue;
		Pv pv;
		Util::uint64 hash;
		bool isNullMoveMaked;
		bool check;
		bool shek;

	public:
		Node() : pgen(NULL), pmove(NULL), isNullMoveMaked(false) {
		}

		Node(const Shogi::Position& pos, const Evaluates::Param& param,
				const History& history) : pgen(NULL), pmove(NULL),
				isNullMoveMaked(false) {
			init(pos, param, history);
		}

		virtual ~Node() {
			if (pgen != NULL) {
				delete pgen;
			}
		}

		void init(const Shogi::Position& pos,
				const Evaluates::Param& param,
				const History& history) {
			if (pgen != NULL) { delete pgen; }
			pgen = new PhasedMoveGenerator(pos, param, history);
			pmove = NULL;
			isNullMoveMaked = false;
		}

		void arrive(Util::uint64 hash, bool check) {
			pv.init();
			pgen->clear();
			this->hash = hash;
			this->check = check;
		}

		void initKiller() {
			pgen->getKiller().init();
		}

		void addKiller(const Shogi::Move& move, const Evaluates::Value& value) {
			pgen->getKiller().add(move, value);
		}

		void setKiller(const Node& node) {
			pgen->getKiller() = node.pgen->getKiller();
		}

		Killer& getKiller() {
			return pgen->getKiller();
		}

		int setPv(const Node& node) {
			return pv.set(*pmove, node.pv);
		}

		int setPv(const Shogi::Move& move, const Node& node) {
			return pv.set(move, node.pv);
		}

		int setPv(const Shogi::Move& move) {
			return pv.set(move);
		}

		const Pv& getPv() const {
			return pv;
		}

		void generateMovesAtOnce() const {
			pgen->generateAll();
		}

		void generateMoves() {
			pgen->init();
			pmove = NULL;
		}

		void generateTacticalMoves() {
			pgen->initTac();
			pmove = NULL;
		}

		void generateCaptures() {
			pgen->initCap();
			pmove = NULL;
		}

		void generateCheck() {
			pgen->initChk();
			pmove = NULL;
		}

		void setHashMove(const HashMove& hashMove) {
			pgen->setHashMove(hashMove);
		}

		bool isHashMove() const {
			return pgen->isHashMove();
		}

		void setMoveIndex(int index) {
			pgen->setCurrent(index);
		}

		int getMoveIndex() const {
			return pgen->getCurrent();
		}

		int getNumberOfMoves() const {
			return pgen->getNumber();
		}

		bool next() {
			return (pmove = pgen->next()) != NULL;
		}

		void makeMove(Shogi::Position& pos,
				Evaluates::Evaluate& eval,
				bool doEval = true) {
			baseValue = eval.getBaseValue();
			posValue = eval.getPositionalValue();
			if (doEval) {
				pos.moveUnsafe(*pmove, change, eval);
			} else {
				pos.moveUnsafe(*pmove, change);
			}
		}

		bool nullMove(Shogi::Position& pos,
				Evaluates::Evaluate& eval) {
			isNullMoveMaked = true;
			baseValue = eval.getBaseValue();
			posValue = eval.getPositionalValue();
			return pos.nullMove(change);
		}

		void unmakeMove(Shogi::Position& pos,
				Evaluates::Evaluate& eval) {
			isNullMoveMaked = false;
			eval.setBaseValue(baseValue);
			eval.setPositionalValue(posValue);
			pos.back(change);
		}

		const Shogi::Move& getMove(int index) const {
			return pgen->get(index);
		}

		const Shogi::Move* getMove() const {
			return pmove;
		}

		void setMove(const Shogi::Move* pmove) {
			this->pmove = pmove;
		}

		bool isNullMove() const {
			return isNullMoveMaked;
		}

		const Shogi::Change& getChange() const {
			return change;
		}

		void addHistory(History& history, int depth) const {
			addHistory(history, depth, pgen->getCurrent());
		}

		void addHistory(History& history, int depth, int index) const {
			assert((unsigned)index-1 <= pgen->getCurrent());
			assert((unsigned)index-1 < pgen->getNumber());
			for (int i = 0; i < index-1; i++) {
				history.add(pgen->get(i), depth, 0);
			}
			history.add(pgen->get(index-1), depth, depth);
		}

		void sort(Evaluates::Value values[]) {
			pgen->sort(values);
		}

		void setShek(bool shek) {
			this->shek = shek;
		}

		bool isShek() const {
			return shek;
		}

		Util::uint64 getHash() const {
			return hash;
		}

		bool isCheck() const {
			return check;
		}
	};
}

#endif // NODE_H_
