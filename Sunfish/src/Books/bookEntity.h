/*
 * bookEntity.h
 *
 *  Created on: 2013/03/10
 *      Author: ryosuke
 */

#ifndef BOOKENTITY_H_
#define BOOKENTITY_H_

#include "bookMoves.h"
#include <list>

namespace Books {
	class BookEntity {
	private:
		std::list<BookMoves> chain;

		BookMoves* _getMoves(Util::uint64 hash) {
			std::list<BookMoves>::iterator it = chain.begin();
			for (; it != chain.end(); it++) {
				if ((*it).getHash() == hash) {
					return &(*it);
				}
			}
			return NULL;
		}

	public:
		void init(unsigned) {
		}

		int addMove(Util::uint64 hash,
				const Shogi::Move& move, unsigned threshold) {
			BookMoves* p = _getMoves(hash);
			if (p != NULL) {
				return p->addMove(move);
			} else {
				putMoves(hash, threshold).addMove(move);
				return 1;
			}
		}

		void setMove(Util::uint64 hash, const Shogi::Move& move,
				unsigned count, unsigned threshold,
				bool overwrite = true) {
			BookMoves* p;
			if (overwrite && (p = _getMoves(hash)) != NULL) {
				p->setMove(move, count);
			} else {
				putMoves(hash, threshold).setMove(move, count);
			}
		}

		BookMoves& putMoves(Util::uint64 hash,
				unsigned threshold, bool overwrite = true) {
			BookMoves* p;
			if (overwrite && (p = _getMoves(hash)) != NULL) {
				return *p;
			}
			chain.push_back(BookMovesBase(hash, threshold));
			return (*chain.rbegin());
		}

		const Shogi::Move* getMove(Util::uint64 hash,
				Util::Random& random) const {
			const BookMoves* p = getMoves(hash);
			if (p != NULL) {
				return p->getMove(random);
			}
			return NULL;
		}

		unsigned getSize() const {
			return chain.size();
		}

		const BookMoves* getMoves(Util::uint64 hash) const {
			std::list<BookMoves>::const_iterator it = chain.begin();
			for (; it != chain.end(); it++) {
				if ((*it).getHash() == hash) {
					return &(*it);
				}
			}
			return NULL;
		}

		const std::list<BookMoves>& getChain() const {
			return chain;
		}
	};
}

#endif // BOOKENTITY_H_
