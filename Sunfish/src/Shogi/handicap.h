/*
 * handicap.h
 *
 *  Created on: 2012/05/11
 *      Author: ryosuke
 */

#ifndef HANDICAP_H_
#define HANDICAP_H_

namespace Shogi {
	enum Handicap {
		NOPIECES = 0,
		EVEN,
		HANDICAP_LANCE,
		HANDICAP_BISHOP,
		HANDICAO_ROOK,
		HANDICAP_ROOK_LANCE,
		HANDICAP_2PIECES,
		HANDICAP_4PIECES,
		HANDICAP_6PIECES,
		HANDICAP_8PIECES,
	};
}

#endif /* HANDICAP_H_ */
