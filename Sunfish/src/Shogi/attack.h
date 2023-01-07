/*
 * attack.h
 *
 *  Created on: 2012/10/07
 *      Author: ryosuke
 */

#ifndef ATTACK_H_
#define ATTACK_H_

#include "piece.h"
#include "square.h"

namespace Shogi {
	class Attack {
	private:
		static const int _attackTable[][17][17];
		static const int (*attackTable)[17][17];
		static const unsigned _checkTable[][17][17];
		static const unsigned (*checkTable)[17][17];


		Attack() {
		}

	public:
		static int attack(Piece piece, int rank, int file) {
			return attackTable[piece][rank][file];
		}

		static unsigned check(Piece piece, int rank, int file) {
			return checkTable[piece][rank][file];
		}

	};
}

#endif // ATTACK_H_
