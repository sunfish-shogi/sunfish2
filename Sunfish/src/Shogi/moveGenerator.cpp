/*
 * moveGenerator.cpp
 *
 *  Created on: 2012/05/13
 *      Author: ryosuke
 */

#include <cassert>
#include <iostream>
#include "../Tools/debug.h"
#include "square.h"
#include "moveGenerator.h"
#include "attack.h"

namespace Shogi {
	template <bool genCap, bool genNocap, bool full>
	unsigned MoveGenerator::generate() {
		if (pos.isCheck()) {
			if (pos.isBlackTurn()) {
				generateEvasion<true, genCap, genNocap, false, full>();
				generateKing<true, true, genCap, genNocap, false>();
			} else {
				generateEvasion<false, genCap, genNocap, false, full>();
				generateKing<false, true, genCap, genNocap, false>();
			}
		} else {
			if (pos.isBlackTurn()) {
				generateOnBoard<true, genCap, genNocap, false, full>();
				generateKing<true, false, genCap, genNocap, false>();
				if (genNocap) { generateDrop<true>(); }
			} else {
				generateOnBoard<false, genCap, genNocap, false, full>();
				generateKing<false, false, genCap, genNocap, false>();
				if (genNocap) { generateDrop<false>(); }
			}
		}
		return num;
	}
	template unsigned MoveGenerator::generate<true, true, true>();
	template unsigned MoveGenerator::generate<true, true, false>();
	template unsigned MoveGenerator::generate<true, false, true>();
	template unsigned MoveGenerator::generate<true, false, false>();
	template unsigned MoveGenerator::generate<false, true, true>();
	template unsigned MoveGenerator::generate<false, true, false>();

	unsigned MoveGenerator::generateTactical() {
		if (pos.isCheck()) {
			if (pos.isBlackTurn()) {
				generateEvasion<true, true, true, false, false>();
				generateKing<true, true, true, true, false>();
			} else {
				generateEvasion<false, true, true, false, false>();
				generateKing<false, true, true, true, false>();
			}
		} else {
			if (pos.isBlackTurn()) {
				generateOnBoard<true, true, false, true, false>();
				generateKing<true, false, true, false, false>();
			} else {
				generateOnBoard<false, true, false, true, false>();
				generateKing<false, false, true, false, false>();
			}
		}
		return num;
	}

	template <bool black, bool genCap, bool genNocap, bool genPro, bool full>
	void MoveGenerator::generateOnBoard() {
		for (Square sq = Square::TOP; sq.valid(); sq.next()) {
			const Piece& piece = pos.getBoard(sq);
			const Direction pin = pos.pin(sq, black).toDirection();
			if (black && piece.isBlack()) {
				switch (piece.getInteger()) {
				case Piece::BPAWN:
					generateStraight<true, true, true, genCap, genNocap, genPro, full>(Piece::BPAWN, sq, Direction::UP, pin);
					break;
				case Piece::BLANCE:
					generateStraight<true, false, true, genCap, genNocap, genPro, true>(Piece::BLANCE, sq, Direction::UP, pin);
					break;
				case Piece::BKNIGHT:
					generateStraight<true, true, true, genCap, genNocap, genPro, true>(Piece::BKNIGHT, sq, Direction::LEFT_UP2, pin);
					generateStraight<true, true, true, genCap, genNocap, genPro, true>(Piece::BKNIGHT, sq, Direction::RIGHT_UP2, pin);
					break;
				case Piece::BSILVER:
					generateStraight<true, true, true, genCap, genNocap, genPro, true>(Piece::BSILVER, sq, Direction::LEFT_UP, pin);
					generateStraight<true, true, true, genCap, genNocap, genPro, true>(Piece::BSILVER, sq, Direction::UP, pin);
					generateStraight<true, true, true, genCap, genNocap, genPro, true>(Piece::BSILVER, sq, Direction::RIGHT_UP, pin);
					generateStraight<true, true, true, genCap, genNocap, genPro, true>(Piece::BSILVER, sq, Direction::LEFT_DOWN, pin);
					generateStraight<true, true, true, genCap, genNocap, genPro, true>(Piece::BSILVER, sq, Direction::RIGHT_DOWN, pin);
					break;
				case Piece::BGOLD:
					generateStraight<true, true, false, genCap, genNocap, genPro, true>(Piece::BGOLD, sq, Direction::LEFT_UP, pin);
					generateStraight<true, true, false, genCap, genNocap, genPro, true>(Piece::BGOLD, sq, Direction::UP, pin);
					generateStraight<true, true, false, genCap, genNocap, genPro, true>(Piece::BGOLD, sq, Direction::RIGHT_UP, pin);
					generateStraight<true, true, false, genCap, genNocap, genPro, true>(Piece::BGOLD, sq, Direction::LEFT, pin);
					generateStraight<true, true, false, genCap, genNocap, genPro, true>(Piece::BGOLD, sq, Direction::RIGHT, pin);
					generateStraight<true, true, false, genCap, genNocap, genPro, true>(Piece::BGOLD, sq, Direction::DOWN, pin);
					break;
				case Piece::BBISHOP:
					generateStraight<true, false, true, genCap, genNocap, genPro, full>(Piece::BBISHOP, sq, Direction::LEFT_UP, pin);
					generateStraight<true, false, true, genCap, genNocap, genPro, full>(Piece::BBISHOP, sq, Direction::RIGHT_UP, pin);
					generateStraight<true, false, true, genCap, genNocap, genPro, full>(Piece::BBISHOP, sq, Direction::LEFT_DOWN, pin);
					generateStraight<true, false, true, genCap, genNocap, genPro, full>(Piece::BBISHOP, sq, Direction::RIGHT_DOWN, pin);
					break;
				case Piece::BROOK:
					generateStraight<true, false, true, genCap, genNocap, genPro, full>(Piece::BROOK, sq, Direction::UP, pin);
					generateStraight<true, false, true, genCap, genNocap, genPro, full>(Piece::BROOK, sq, Direction::DOWN, pin);
					generateStraight<true, false, true, genCap, genNocap, genPro, full>(Piece::BROOK, sq, Direction::LEFT, pin);
					generateStraight<true, false, true, genCap, genNocap, genPro, full>(Piece::BROOK, sq, Direction::RIGHT, pin);
					break;
				case Piece::BTOKIN:
					generateStraight<true, true, false, genCap, genNocap, genPro, true>(Piece::BTOKIN, sq, Direction::LEFT_UP, pin);
					generateStraight<true, true, false, genCap, genNocap, genPro, true>(Piece::BTOKIN, sq, Direction::UP, pin);
					generateStraight<true, true, false, genCap, genNocap, genPro, true>(Piece::BTOKIN, sq, Direction::RIGHT_UP, pin);
					generateStraight<true, true, false, genCap, genNocap, genPro, true>(Piece::BTOKIN, sq, Direction::LEFT, pin);
					generateStraight<true, true, false, genCap, genNocap, genPro, true>(Piece::BTOKIN, sq, Direction::RIGHT, pin);
					generateStraight<true, true, false, genCap, genNocap, genPro, true>(Piece::BTOKIN, sq, Direction::DOWN, pin);
					break;
				case Piece::BPLANCE:
					generateStraight<true, true, false, genCap, genNocap, genPro, true>(Piece::BPLANCE, sq, Direction::LEFT_UP, pin);
					generateStraight<true, true, false, genCap, genNocap, genPro, true>(Piece::BPLANCE, sq, Direction::UP, pin);
					generateStraight<true, true, false, genCap, genNocap, genPro, true>(Piece::BPLANCE, sq, Direction::RIGHT_UP, pin);
					generateStraight<true, true, false, genCap, genNocap, genPro, true>(Piece::BPLANCE, sq, Direction::LEFT, pin);
					generateStraight<true, true, false, genCap, genNocap, genPro, true>(Piece::BPLANCE, sq, Direction::RIGHT, pin);
					generateStraight<true, true, false, genCap, genNocap, genPro, true>(Piece::BPLANCE, sq, Direction::DOWN, pin);
					break;
				case Piece::BPKNIGHT:
					generateStraight<true, true, false, genCap, genNocap, genPro, true>(Piece::BPKNIGHT, sq, Direction::LEFT_UP, pin);
					generateStraight<true, true, false, genCap, genNocap, genPro, true>(Piece::BPKNIGHT, sq, Direction::UP, pin);
					generateStraight<true, true, false, genCap, genNocap, genPro, true>(Piece::BPKNIGHT, sq, Direction::RIGHT_UP, pin);
					generateStraight<true, true, false, genCap, genNocap, genPro, true>(Piece::BPKNIGHT, sq, Direction::LEFT, pin);
					generateStraight<true, true, false, genCap, genNocap, genPro, true>(Piece::BPKNIGHT, sq, Direction::RIGHT, pin);
					generateStraight<true, true, false, genCap, genNocap, genPro, true>(Piece::BPKNIGHT, sq, Direction::DOWN, pin);
					break;
				case Piece::BPSILVER:
					generateStraight<true, true, false, genCap, genNocap, genPro, true>(Piece::BPSILVER, sq, Direction::LEFT_UP, pin);
					generateStraight<true, true, false, genCap, genNocap, genPro, true>(Piece::BPSILVER, sq, Direction::UP, pin);
					generateStraight<true, true, false, genCap, genNocap, genPro, true>(Piece::BPSILVER, sq, Direction::RIGHT_UP, pin);
					generateStraight<true, true, false, genCap, genNocap, genPro, true>(Piece::BPSILVER, sq, Direction::LEFT, pin);
					generateStraight<true, true, false, genCap, genNocap, genPro, true>(Piece::BPSILVER, sq, Direction::RIGHT, pin);
					generateStraight<true, true, false, genCap, genNocap, genPro, true>(Piece::BPSILVER, sq, Direction::DOWN, pin);
					break;
				case Piece::BHORSE:
					generateStraight<true, false, false, genCap, genNocap, genPro, true>(Piece::BHORSE, sq, Direction::LEFT_UP, pin);
					generateStraight<true, false, false, genCap, genNocap, genPro, true>(Piece::BHORSE, sq, Direction::RIGHT_UP, pin);
					generateStraight<true, false, false, genCap, genNocap, genPro, true>(Piece::BHORSE, sq, Direction::LEFT_DOWN, pin);
					generateStraight<true, false, false, genCap, genNocap, genPro, true>(Piece::BHORSE, sq, Direction::RIGHT_DOWN, pin);
					generateStraight<true, true, false, genCap, genNocap, genPro, true>(Piece::BHORSE, sq, Direction::UP, pin);
					generateStraight<true, true, false, genCap, genNocap, genPro, true>(Piece::BHORSE, sq, Direction::LEFT, pin);
					generateStraight<true, true, false, genCap, genNocap, genPro, true>(Piece::BHORSE, sq, Direction::RIGHT, pin);
					generateStraight<true, true, false, genCap, genNocap, genPro, true>(Piece::BHORSE, sq, Direction::DOWN, pin);
					break;
				case Piece::BDRAGON:
					generateStraight<true, false, false, genCap, genNocap, genPro, true>(Piece::BDRAGON, sq, Direction::UP, pin);
					generateStraight<true, false, false, genCap, genNocap, genPro, true>(Piece::BDRAGON, sq, Direction::DOWN, pin);
					generateStraight<true, false, false, genCap, genNocap, genPro, true>(Piece::BDRAGON, sq, Direction::LEFT, pin);
					generateStraight<true, false, false, genCap, genNocap, genPro, true>(Piece::BDRAGON, sq, Direction::RIGHT, pin);
					generateStraight<true, true, false, genCap, genNocap, genPro, true>(Piece::BDRAGON, sq, Direction::LEFT_UP, pin);
					generateStraight<true, true, false, genCap, genNocap, genPro, true>(Piece::BDRAGON, sq, Direction::RIGHT_UP, pin);
					generateStraight<true, true, false, genCap, genNocap, genPro, true>(Piece::BDRAGON, sq, Direction::LEFT_DOWN, pin);
					generateStraight<true, true, false, genCap, genNocap, genPro, true>(Piece::BDRAGON, sq, Direction::RIGHT_DOWN, pin);
					break;
				}
			} else if (!black && piece.isWhite()) {
				switch (piece.getInteger()) {
				case Piece::WPAWN:
					generateStraight<false, true, true, genCap, genNocap, genPro, full>(Piece::WPAWN, sq, Direction::UP_R, pin);
					break;
				case Piece::WLANCE:
					generateStraight<false, false, true, genCap, genNocap, genPro, true>(Piece::WLANCE, sq, Direction::UP_R, pin);
					break;
				case Piece::WKNIGHT:
					generateStraight<false, true, true, genCap, genNocap, genPro, true>(Piece::WKNIGHT, sq, Direction::LEFT_UP2_R, pin);
					generateStraight<false, true, true, genCap, genNocap, genPro, true>(Piece::WKNIGHT, sq, Direction::RIGHT_UP2_R, pin);
					break;
				case Piece::WSILVER:
					generateStraight<false, true, true, genCap, genNocap, genPro, true>(Piece::WSILVER, sq, Direction::LEFT_UP_R, pin);
					generateStraight<false, true, true, genCap, genNocap, genPro, true>(Piece::WSILVER, sq, Direction::UP_R, pin);
					generateStraight<false, true, true, genCap, genNocap, genPro, true>(Piece::WSILVER, sq, Direction::RIGHT_UP_R, pin);
					generateStraight<false, true, true, genCap, genNocap, genPro, true>(Piece::WSILVER, sq, Direction::LEFT_DOWN_R, pin);
					generateStraight<false, true, true, genCap, genNocap, genPro, true>(Piece::WSILVER, sq, Direction::RIGHT_DOWN_R, pin);
					break;
				case Piece::WGOLD:
					generateStraight<false, true, false, genCap, genNocap, genPro, true>(Piece::WGOLD, sq, Direction::LEFT_UP_R, pin);
					generateStraight<false, true, false, genCap, genNocap, genPro, true>(Piece::WGOLD, sq, Direction::UP_R, pin);
					generateStraight<false, true, false, genCap, genNocap, genPro, true>(Piece::WGOLD, sq, Direction::RIGHT_UP_R, pin);
					generateStraight<false, true, false, genCap, genNocap, genPro, true>(Piece::WGOLD, sq, Direction::LEFT_R, pin);
					generateStraight<false, true, false, genCap, genNocap, genPro, true>(Piece::WGOLD, sq, Direction::RIGHT_R, pin);
					generateStraight<false, true, false, genCap, genNocap, genPro, true>(Piece::WGOLD, sq, Direction::DOWN_R, pin);
					break;
				case Piece::WBISHOP:
					generateStraight<false, false, true, genCap, genNocap, genPro, full>(Piece::WBISHOP, sq, Direction::LEFT_UP_R, pin);
					generateStraight<false, false, true, genCap, genNocap, genPro, full>(Piece::WBISHOP, sq, Direction::RIGHT_UP_R, pin);
					generateStraight<false, false, true, genCap, genNocap, genPro, full>(Piece::WBISHOP, sq, Direction::LEFT_DOWN_R, pin);
					generateStraight<false, false, true, genCap, genNocap, genPro, full>(Piece::WBISHOP, sq, Direction::RIGHT_DOWN_R, pin);
					break;
				case Piece::WROOK:
					generateStraight<false, false, true, genCap, genNocap, genPro, full>(Piece::WROOK, sq, Direction::UP_R, pin);
					generateStraight<false, false, true, genCap, genNocap, genPro, full>(Piece::WROOK, sq, Direction::DOWN_R, pin);
					generateStraight<false, false, true, genCap, genNocap, genPro, full>(Piece::WROOK, sq, Direction::LEFT_R, pin);
					generateStraight<false, false, true, genCap, genNocap, genPro, full>(Piece::WROOK, sq, Direction::RIGHT_R, pin);
					break;
				case Piece::WTOKIN:
					generateStraight<false, true, false, genCap, genNocap, genPro, true>(Piece::WTOKIN, sq, Direction::LEFT_UP_R, pin);
					generateStraight<false, true, false, genCap, genNocap, genPro, true>(Piece::WTOKIN, sq, Direction::UP_R, pin);
					generateStraight<false, true, false, genCap, genNocap, genPro, true>(Piece::WTOKIN, sq, Direction::RIGHT_UP_R, pin);
					generateStraight<false, true, false, genCap, genNocap, genPro, true>(Piece::WTOKIN, sq, Direction::LEFT_R, pin);
					generateStraight<false, true, false, genCap, genNocap, genPro, true>(Piece::WTOKIN, sq, Direction::RIGHT_R, pin);
					generateStraight<false, true, false, genCap, genNocap, genPro, true>(Piece::WTOKIN, sq, Direction::DOWN_R, pin);
					break;
				case Piece::WPLANCE:
					generateStraight<false, true, false, genCap, genNocap, genPro, true>(Piece::WPLANCE, sq, Direction::LEFT_UP_R, pin);
					generateStraight<false, true, false, genCap, genNocap, genPro, true>(Piece::WPLANCE, sq, Direction::UP_R, pin);
					generateStraight<false, true, false, genCap, genNocap, genPro, true>(Piece::WPLANCE, sq, Direction::RIGHT_UP_R, pin);
					generateStraight<false, true, false, genCap, genNocap, genPro, true>(Piece::WPLANCE, sq, Direction::LEFT_R, pin);
					generateStraight<false, true, false, genCap, genNocap, genPro, true>(Piece::WPLANCE, sq, Direction::RIGHT_R, pin);
					generateStraight<false, true, false, genCap, genNocap, genPro, true>(Piece::WPLANCE, sq, Direction::DOWN_R, pin);
					break;
				case Piece::WPKNIGHT:
					generateStraight<false, true, false, genCap, genNocap, genPro, true>(Piece::WPKNIGHT, sq, Direction::LEFT_UP_R, pin);
					generateStraight<false, true, false, genCap, genNocap, genPro, true>(Piece::WPKNIGHT, sq, Direction::UP_R, pin);
					generateStraight<false, true, false, genCap, genNocap, genPro, true>(Piece::WPKNIGHT, sq, Direction::RIGHT_UP_R, pin);
					generateStraight<false, true, false, genCap, genNocap, genPro, true>(Piece::WPKNIGHT, sq, Direction::LEFT_R, pin);
					generateStraight<false, true, false, genCap, genNocap, genPro, true>(Piece::WPKNIGHT, sq, Direction::RIGHT_R, pin);
					generateStraight<false, true, false, genCap, genNocap, genPro, true>(Piece::WPKNIGHT, sq, Direction::DOWN_R, pin);
					break;
				case Piece::WPSILVER:
					generateStraight<false, true, false, genCap, genNocap, genPro, true>(Piece::WPSILVER, sq, Direction::LEFT_UP_R, pin);
					generateStraight<false, true, false, genCap, genNocap, genPro, true>(Piece::WPSILVER, sq, Direction::UP_R, pin);
					generateStraight<false, true, false, genCap, genNocap, genPro, true>(Piece::WPSILVER, sq, Direction::RIGHT_UP_R, pin);
					generateStraight<false, true, false, genCap, genNocap, genPro, true>(Piece::WPSILVER, sq, Direction::LEFT_R, pin);
					generateStraight<false, true, false, genCap, genNocap, genPro, true>(Piece::WPSILVER, sq, Direction::RIGHT_R, pin);
					generateStraight<false, true, false, genCap, genNocap, genPro, true>(Piece::WPSILVER, sq, Direction::DOWN_R, pin);
					break;
				case Piece::WHORSE:
					generateStraight<false, false, false, genCap, genNocap, genPro, true>(Piece::WHORSE, sq, Direction::LEFT_UP_R, pin);
					generateStraight<false, false, false, genCap, genNocap, genPro, true>(Piece::WHORSE, sq, Direction::RIGHT_UP_R, pin);
					generateStraight<false, false, false, genCap, genNocap, genPro, true>(Piece::WHORSE, sq, Direction::LEFT_DOWN_R, pin);
					generateStraight<false, false, false, genCap, genNocap, genPro, true>(Piece::WHORSE, sq, Direction::RIGHT_DOWN_R, pin);
					generateStraight<false, true, false, genCap, genNocap, genPro, true>(Piece::WHORSE, sq, Direction::UP_R, pin);
					generateStraight<false, true, false, genCap, genNocap, genPro, true>(Piece::WHORSE, sq, Direction::LEFT_R, pin);
					generateStraight<false, true, false, genCap, genNocap, genPro, true>(Piece::WHORSE, sq, Direction::RIGHT_R, pin);
					generateStraight<false, true, false, genCap, genNocap, genPro, true>(Piece::WHORSE, sq, Direction::DOWN_R, pin);
					break;
				case Piece::WDRAGON:
					generateStraight<false, false, false, genCap, genNocap, genPro, true>(Piece::WDRAGON, sq, Direction::UP_R, pin);
					generateStraight<false, false, false, genCap, genNocap, genPro, true>(Piece::WDRAGON, sq, Direction::DOWN_R, pin);
					generateStraight<false, false, false, genCap, genNocap, genPro, true>(Piece::WDRAGON, sq, Direction::LEFT_R, pin);
					generateStraight<false, false, false, genCap, genNocap, genPro, true>(Piece::WDRAGON, sq, Direction::RIGHT_R, pin);
					generateStraight<false, true, false, genCap, genNocap, genPro, true>(Piece::WDRAGON, sq, Direction::LEFT_UP_R, pin);
					generateStraight<false, true, false, genCap, genNocap, genPro, true>(Piece::WDRAGON, sq, Direction::RIGHT_UP_R, pin);
					generateStraight<false, true, false, genCap, genNocap, genPro, true>(Piece::WDRAGON, sq, Direction::LEFT_DOWN_R, pin);
					generateStraight<false, true, false, genCap, genNocap, genPro, true>(Piece::WDRAGON, sq, Direction::RIGHT_DOWN_R, pin);
					break;
				}
			}
		}
	}

	template <bool black, bool oneStep, bool promotable, bool genCap, bool genNocap, bool genPro, bool full>
	void MoveGenerator::generateStraight(const Piece piece, const Square from, const Direction dir, const Direction pin) {
		if (pin != Direction::NON && dir != pin && dir != pin.reverse()) {
			return;
		}
		Square to = from + dir;
		while (true) {
			const Piece& piece2 = pos.getBoard(to);
			if (piece2.isWall() || (black && piece2.isBlack()) || (!black && piece2.isWhite())) {
				break;
			}
			if ((genCap && !piece2.isEmpty()) || (genNocap && piece2.isEmpty())) {
				bool promoted = false;
				if (promotable) {
					promoted = generateOneMovePro<black>(piece, from, to);
				}
				if (full || !promoted) {
					generateOneMove<black>(piece, from, to);
				}
			} else if (promotable && genPro) {
				generateOneMovePro<black>(piece, from, to);
			}
			if(oneStep) {
				break;
			}
			if (!piece2.isEmpty()) {
				break;
			}
			to += dir;
		}
	}

	template <bool black, bool check, bool genCap, bool genNocap, bool genCheckOnly>
	void MoveGenerator::generateKing() {
		if (black ? pos.getBKing().valid() : pos.getWKing().valid()) {
			generateKingDirection<black, check, genCap, genNocap, genCheckOnly>(Direction::LEFT_UP);
			generateKingDirection<black, check, genCap, genNocap, genCheckOnly>(Direction::UP);
			generateKingDirection<black, check, genCap, genNocap, genCheckOnly>(Direction::RIGHT_UP);
			generateKingDirection<black, check, genCap, genNocap, genCheckOnly>(Direction::LEFT);
			generateKingDirection<black, check, genCap, genNocap, genCheckOnly>(Direction::RIGHT);
			generateKingDirection<black, check, genCap, genNocap, genCheckOnly>(Direction::LEFT_DOWN);
			generateKingDirection<black, check, genCap, genNocap, genCheckOnly>(Direction::DOWN);
			generateKingDirection<black, check, genCap, genNocap, genCheckOnly>(Direction::RIGHT_DOWN);
		}
	}

	template <bool black, bool check, bool genCap, bool genNocap, bool genCheckOnly>
	void MoveGenerator::generateKingDirection(const Direction& dir) {
		Square from = (black ? pos.getBKing() : pos.getWKing());
		Square to = from + dir;
		Piece piece = pos.getBoard(to);
		if (!genCap && !piece.isEmpty()) { // 駒を取る手
			return;
		}
		if (!genNocap && piece.isEmpty()) { // 駒を取らない手
			return;
		}
		DirectionFlags effectTo = pos.getEffect(to, !black);
		DirectionFlags effectFrom = pos.getEffect(from, !black);
		if ((black && !piece.isBlackMovable()) || (!black && !piece.isWhiteMovable())
				|| effectTo.longOrShortRange() || (check && effectFrom.getLongRangeOnly().check(dir, true))) {
			return;
		}
		if (genCheckOnly && !pos.isCheckMoveDiscovered(from, to)) {
			return;
		}
		moves[num++] = Move(from, to, false, false, (black ? Piece::BKING : Piece::WKING));
	}

	template <bool black>
	void MoveGenerator::generateDrop() {
		generateDropPieces<black, (black ? Piece::BPAWN : Piece::WPAWN)>();
		generateDropPieces<black, (black ? Piece::BLANCE : Piece::WLANCE)>();
		generateDropPieces<black, (black ? Piece::BKNIGHT : Piece::WKNIGHT)>();
		generateDropPieces<black, (black ? Piece::BSILVER : Piece::WSILVER)>();
		generateDropPieces<black, (black ? Piece::BGOLD : Piece::WGOLD)>();
		generateDropPieces<black, (black ? Piece::BBISHOP : Piece::WBISHOP)>();
		generateDropPieces<black, (black ? Piece::BROOK : Piece::WROOK)>();
	}

	template <bool black, unsigned piece>
	void MoveGenerator::generateDropPieces() {
		assert(black != (piece & Piece::TURN));
		if ((black ? pos.getBlackHand(piece) : pos.getWhiteHand(piece)) != 0) {
			for (unsigned file = 1; file <= Square::FILE_NUM; file++) {
				if (black && piece == Piece::BPAWN && pos.getBPawnFiles().exist(file)) {
					continue;
				}
				if (!black && piece == Piece::WPAWN && pos.getWPawnFiles().exist(file)) {
					continue;
				}
				unsigned rank_top = 1;
				unsigned rank_bottom = Square::RANK_NUM;
				if (piece == Piece::BPAWN || piece == Piece::BLANCE) {
					rank_top = 2;
				} else if (piece == Piece::BKNIGHT) {
					rank_top = 3;
				} else if (piece == Piece::WPAWN || piece == Piece::WLANCE) {
					rank_bottom = Square::RANK_NUM - 1;
				} else if (piece == Piece::WKNIGHT) {
					rank_bottom = Square::RANK_NUM - 2;
				}
				for (unsigned rank = rank_top; rank <= rank_bottom; rank++) {
					Square sq(file, rank);
					if (pos.getBoard(sq).isEmpty()) {
						if (black && piece == Piece::BPAWN && pos.isPawnDropMate(sq, true)) {
							continue;
						} else if (!black && piece == Piece::WPAWN && pos.isPawnDropMate(sq, false)) {
							continue;
						}
						moves[num++] = Move(Square::NON, sq, false, true, piece);
					}
				}
			}
		}
	}

	template <bool black, bool genCap, bool genNocap, bool genCheckOnly, bool full>
	void MoveGenerator::generateEvasion() {
		DirectionFlags flags = pos.getCheckDirection();
		if (!flags.isPlural()) { // 両王手でない場合
			Direction dir = flags.toDirection().reverse();
			Square sq = (black ? pos.getBKing() : pos.getWKing());
			for (sq += dir; pos.getBoard(sq) == Piece::EMPTY; sq += dir) {
				if (genNocap) { // 駒を取らない手
					generateEvasionOnBoard<black, genCheckOnly, full>(sq);
					generateEvasionDrop<black, (black ? Piece::BPAWN : Piece::WPAWN), genCheckOnly>(sq);
					generateEvasionDrop<black, (black ? Piece::BLANCE : Piece::WLANCE), genCheckOnly>(sq);
					generateEvasionDrop<black, (black ? Piece::BKNIGHT : Piece::WKNIGHT), genCheckOnly>(sq);
					generateEvasionDrop<black, (black ? Piece::BSILVER : Piece::WSILVER), genCheckOnly>(sq);
					generateEvasionDrop<black, (black ? Piece::BGOLD : Piece::WGOLD), genCheckOnly>(sq);
					generateEvasionDrop<black, (black ? Piece::BBISHOP : Piece::WBISHOP), genCheckOnly>(sq);
					generateEvasionDrop<black, (black ? Piece::BROOK : Piece::WROOK), genCheckOnly>(sq);
				}
			}
			if (genCap) { // 駒を取る手
				generateEvasionOnBoard<black, genCheckOnly, full>(sq);
			}
		}
	}

	template <bool black, bool genCheckOnly, bool full>
	void MoveGenerator::generateEvasionOnBoard(Square to) {
		DirectionFlags flags = pos.getEffect(to, black).getExcludeKing();
		while (flags.isNonZero()) {
			Direction dir = flags.pop().toDirection().reverse();
			Square from = to;
			for (from += dir; pos.getBoard(from) == Piece::EMPTY; from += dir)
				;
			Piece piece = pos.getBoard(from);
			if (!piece.isKing<black>() && pos.pin(from, black).isZero()) {
				bool promoted = false;
				if (piece.isPromotable() && (!genCheckOnly ||
						pos.isCheckMove(from, to, piece, true))) {
					promoted = generateOneMovePro<black>(piece, from, to);
				}
				if ((full || !promoted || !promotionOnly(piece)) &&
						(!genCheckOnly || pos.isCheckMove(from, to, piece, false))) {
					generateOneMove<black>(piece, from, to);
				}
			}
		}
	}

	template <bool black, unsigned piece, bool genCheckOnly>
	void MoveGenerator::generateEvasionDrop(Square to) {
		assert(black != (piece & Piece::TURN));
		if ((black ? pos.getBlackHand(piece) : pos.getWhiteHand(piece)) != 0) {
			if (black && piece == Piece::BPAWN && pos.getBPawnFiles().exist(to.getFile())) {
				return;
			}
			if (!black && piece == Piece::WPAWN && pos.getWPawnFiles().exist(to.getFile())) {
				return;
			}
			if (to.isCompulsoryPromotion(piece)) {
				return;
			}
			if (black && piece == Piece::BPAWN && pos.isPawnDropMate(to, true)) {
				return;
			} else if (!black && piece == Piece::WPAWN && pos.isPawnDropMate(to, false)) {
				return;
			}
			if (genCheckOnly && !pos.isCheckMoveDirect(to, piece, false)) {
				return;
			}
			moves[num++] = Move(Square::NON, to, false, true, piece);
		}
	}

	unsigned MoveGenerator::generateTardy() {
		for (Square from = Square::TOP; from.valid(); from.next()) {
			Piece piece = pos.getBoard(from);
			if (!piece.isEmpty() && piece.isBlack() == pos.isBlackTurn()) {
				for (Square to = Square::TOP; to.valid(); to.next()) {
					Move move(from, to, true, false, piece);
					if (pos.isLegalMove(move)) {
						moves[num++] = move;
						if (promotionOnly(piece)) {
							continue;
						}
					}
					move.setPromotion(false);
					if (pos.isLegalMove(move)) {
						moves[num++] = move;
					}
				}
			}
		}
		for (Piece piece = Piece::PAWN; piece != Piece::KING; piece.toNext()) {
			if (pos.getHand(piece) != 0) {
				Piece drop = pos.isBlackTurn() ? piece : piece.getTurnedWhite();
				for (Square to = Square::TOP; to.valid(); to.next()) {
					Move move(Square::NON, to, false, true, drop);
					if (pos.isLegalMove(move)) {
						moves[num++] = move;
					}
				}
			}
		}
		return num;
	}

	void MoveGenerator::sort(int begin, int size) {
		// TODO: use boost
//		boost::sort(moves);
		for (int i = begin + 1; i < begin + size; i++) {
			Move move = moves[i];
			int j;
			for (j = i - 1; j >= begin; j--) {
				if (move < moves[j]) {
					break;
				}
				moves[j+1] = moves[j];
			}
			moves[j+1] = move;
		}
	}

	bool MoveGenerator::equals(const MoveGenerator gen) const {
		if (num != gen.num) {
			return false;
		}
		for (unsigned i = 0; i < num; i++) {
			if (moves[i] != gen.moves[i]) {
				return false;
			}
		}
		return true;
	}

	unsigned MoveGenerator::generateCheck() {
		if (pos.isCheck()) {
			if (pos.isBlackTurn()) {
				generateEvasion<true, true, true, true, true>();
				generateKing<true, true, true, true, true>();
			} else {
				generateEvasion<false, true, true, true, true>();
				generateKing<false, true, true, true, true>();
			}
		} else {
			if (pos.isBlackTurn()) {
				generateCheckDrop<true>();
				generateCheckOnBoard<true>();
			} else {
				generateCheckDrop<false>();
				generateCheckOnBoard<false>();
			}
		}
		return num;
	}

	template <bool black>
	void MoveGenerator::generateCheckOnBoard() {
		Square king = black ? pos.getWKing() : pos.getBKing();
		for (Square from = Square::TOP; from.valid(); from.next()) {
			const Piece& piece = pos.getBoard(from);
			if ((black && piece.isBlack()) ||
					(!black && piece.isWhite())) {
				const Direction pin = pos.pin(from, black).toDirection();
				// 直接王手
				DirectionFlags flags(Attack::check(piece,
						king.getRank() - from.getRank(),
						king.getFile() - from.getFile()));
				DirectionFlags f = flags.getShortRangeOnly();
				while (f.isNonZero()) {
					Direction dir = f.pop().toDirection();
					if (pin != Direction::NON && dir != pin && dir != pin.reverse()) {
						continue;
					}
					Square to = from + dir;
					Piece piece2 = pos.getBoard(to);
					if ((black && piece2.isBlackMovable()) ||
							(!black && piece2.isWhiteMovable())) {
						generateCheck<black, true>(from, to, piece);
					}
				}
				f = flags.getLongRangeOnly();
				while (f.isNonZero()) {
					Direction dir = f.pop().toDirection();
					if (pin != Direction::NON && dir != pin && dir != pin.reverse()) {
						continue;
					}
					for (Square to = from + dir; ; to += dir) {
						Piece piece2 = pos.getBoard(to);
						if ((black && !piece2.isBlackMovable()) ||
								(!black && !piece2.isWhiteMovable())) {
							break;
						}
						generateCheck<black, true>(from, to, piece);
						if (!piece2.isEmpty()) {
							break;
						}
					}
				}
				// 開き王手
				DirectionFlags effectKing = pos.getEffect(from, !black);
				DirectionFlags attacker = pos.getEffect(from, black);
				if ((attacker = effectKing.isAttackedBy(attacker))) {
					DirectionFlags flags = piece.getMovableDirection();
					Direction attackDir = attacker.toDirection();
					flags.remove(attackDir);
					flags.remove(attackDir.reverse());
					f = flags.getShortRangeOnly();
					while (f.isNonZero()) {
						Direction dir = f.pop().toDirection();
						Square to = from + dir;
						if (piece.isKing() && pos.getEffect(to, !black).longOrShortRange()) {
							continue;
						}
						if (pin != Direction::NON && dir != pin && dir != pin.reverse()) {
							continue;
						}
						Piece piece2 = pos.getBoard(to);
						if ((black && piece2.isBlackMovable()) ||
								(!black && piece2.isWhiteMovable())) {
							generateCheck<black, false>(from, to, piece);
						}
					}
					f = flags.getLongRangeOnly();
					while (f.isNonZero()) {
						Direction dir = f.pop().toDirection();
						if (pin != Direction::NON && dir != pin && dir != pin.reverse()) {
							continue;
						}
						for (Square to = from + dir; ; to += dir) {
							Piece piece2 = pos.getBoard(to);
							if ((black && !piece2.isBlackMovable()) ||
									(!black && !piece2.isWhiteMovable())) {
								break;
							}
							generateCheck<black, false>(from, to, piece);
							if (!piece2.isEmpty()) {
								break;
							}
						}
					}
				}
			}
		}
	}
	template void MoveGenerator::generateCheckOnBoard<true>();
	template void MoveGenerator::generateCheckOnBoard<false>();

	template <bool black, bool direct>
	void MoveGenerator::generateCheck(Square from, Square to, Piece piece) {
		// 不成
		if (!to.isCompulsoryPromotion(piece)) {
			if (pos.isCheckMoveDirect(to, piece, false) == direct) {
				moves[num++] = Move(from, to, false, false, piece);
			}
		}
		// 成り
		if (piece.isPromotable() && (to.isPromotableRank(black) || from.isPromotableRank(black))) {
			if (pos.isCheckMoveDirect(to, piece, true) == direct) {
				moves[num++] = Move(from, to, true, false, piece);
			}
		}
	}

	template void MoveGenerator::generateCheck<true, true>(Square from, Square to, Piece piece);
	template void MoveGenerator::generateCheck<false, true>(Square from, Square to, Piece piece);
	template void MoveGenerator::generateCheck<true, false>(Square from, Square to, Piece piece);
	template void MoveGenerator::generateCheck<false, false>(Square from, Square to, Piece piece);

	template <bool black>
	void MoveGenerator::generateCheckDrop() {
		if (black) {
			if (pos.getBlackHand(Piece::BPAWN) != 0) {
				generateCheckDropStraight<true, Piece::BPAWN, false>(Direction::DOWN);
			}
			if (pos.getBlackHand(Piece::BLANCE) != 0) {
				generateCheckDropStraight<true, Piece::BLANCE, true>(Direction::DOWN);
			}
			if (pos.getBlackHand(Piece::BKNIGHT) != 0) {
				generateCheckDropStraight<true, Piece::BKNIGHT, false>(Direction::RIGHT_DOWN2);
				generateCheckDropStraight<true, Piece::BKNIGHT, false>(Direction::LEFT_DOWN2);
			}
			if (pos.getBlackHand(Piece::BSILVER) != 0) {
				generateCheckDropStraight<true, Piece::BSILVER, false>(Direction::RIGHT_UP);
				generateCheckDropStraight<true, Piece::BSILVER, false>(Direction::LEFT_UP);
				generateCheckDropStraight<true, Piece::BSILVER, false>(Direction::RIGHT_DOWN);
				generateCheckDropStraight<true, Piece::BSILVER, false>(Direction::DOWN);
				generateCheckDropStraight<true, Piece::BSILVER, false>(Direction::LEFT_DOWN);
			}
			if (pos.getBlackHand(Piece::BGOLD) != 0) {
				generateCheckDropStraight<true, Piece::BGOLD, false>(Direction::UP);
				generateCheckDropStraight<true, Piece::BGOLD, false>(Direction::RIGHT);
				generateCheckDropStraight<true, Piece::BGOLD, false>(Direction::LEFT);
				generateCheckDropStraight<true, Piece::BGOLD, false>(Direction::RIGHT_DOWN);
				generateCheckDropStraight<true, Piece::BGOLD, false>(Direction::DOWN);
				generateCheckDropStraight<true, Piece::BGOLD, false>(Direction::LEFT_DOWN);
			}
			if (pos.getBlackHand(Piece::BBISHOP) != 0) {
				generateCheckDropStraight<true, Piece::BBISHOP, true>(Direction::RIGHT_UP);
				generateCheckDropStraight<true, Piece::BBISHOP, true>(Direction::LEFT_UP);
				generateCheckDropStraight<true, Piece::BBISHOP, true>(Direction::RIGHT_DOWN);
				generateCheckDropStraight<true, Piece::BBISHOP, true>(Direction::LEFT_DOWN);
			}
			if (pos.getBlackHand(Piece::BROOK) != 0) {
				generateCheckDropStraight<true, Piece::BROOK, true>(Direction::UP);
				generateCheckDropStraight<true, Piece::BROOK, true>(Direction::RIGHT);
				generateCheckDropStraight<true, Piece::BROOK, true>(Direction::LEFT);
				generateCheckDropStraight<true, Piece::BROOK, true>(Direction::DOWN);
			}
		} else {
			if (pos.getWhiteHand(Piece::WPAWN) != 0) {
				generateCheckDropStraight<false, Piece::WPAWN, false>(Direction::UP);
			}
			if (pos.getWhiteHand(Piece::WLANCE) != 0) {
				generateCheckDropStraight<false, Piece::WLANCE, true>(Direction::UP);
			}
			if (pos.getWhiteHand(Piece::WKNIGHT) != 0) {
				generateCheckDropStraight<false, Piece::WKNIGHT, false>(Direction::LEFT_UP2);
				generateCheckDropStraight<false, Piece::WKNIGHT, false>(Direction::RIGHT_UP2);
			}
			if (pos.getWhiteHand(Piece::WSILVER) != 0) {
				generateCheckDropStraight<false, Piece::WSILVER, false>(Direction::LEFT_DOWN);
				generateCheckDropStraight<false, Piece::WSILVER, false>(Direction::RIGHT_DOWN);
				generateCheckDropStraight<false, Piece::WSILVER, false>(Direction::LEFT_UP);
				generateCheckDropStraight<false, Piece::WSILVER, false>(Direction::UP);
				generateCheckDropStraight<false, Piece::WSILVER, false>(Direction::RIGHT_UP);
			}
			if (pos.getWhiteHand(Piece::WGOLD) != 0) {
				generateCheckDropStraight<false, Piece::WGOLD, false>(Direction::DOWN);
				generateCheckDropStraight<false, Piece::WGOLD, false>(Direction::LEFT);
				generateCheckDropStraight<false, Piece::WGOLD, false>(Direction::RIGHT);
				generateCheckDropStraight<false, Piece::WGOLD, false>(Direction::LEFT_UP);
				generateCheckDropStraight<false, Piece::WGOLD, false>(Direction::UP);
				generateCheckDropStraight<false, Piece::WGOLD, false>(Direction::RIGHT_UP);
			}
			if (pos.getWhiteHand(Piece::WBISHOP) != 0) {
				generateCheckDropStraight<false, Piece::WBISHOP, true>(Direction::LEFT_DOWN);
				generateCheckDropStraight<false, Piece::WBISHOP, true>(Direction::RIGHT_DOWN);
				generateCheckDropStraight<false, Piece::WBISHOP, true>(Direction::LEFT_UP);
				generateCheckDropStraight<false, Piece::WBISHOP, true>(Direction::RIGHT_UP);
			}
			if (pos.getWhiteHand(Piece::WROOK) != 0) {
				generateCheckDropStraight<false, Piece::WROOK, true>(Direction::DOWN);
				generateCheckDropStraight<false, Piece::WROOK, true>(Direction::LEFT);
				generateCheckDropStraight<false, Piece::WROOK, true>(Direction::RIGHT);
				generateCheckDropStraight<false, Piece::WROOK, true>(Direction::UP);
			}
		}
	}

	template <bool black, unsigned piece, bool longRange>
	void MoveGenerator::generateCheckDropStraight(Direction dir) {
		Square king = black ? pos.getWKing() : pos.getBKing();
		// 移動先
		Square to = king + dir;
		Piece p = pos.getBoard(to);
		if (!p.isEmpty()) { return; }
		// 二歩チェック
		if (black && piece == Piece::BPAWN && pos.getBPawnFiles().exist(to.getFile())) {
			return;
		}
		if (!black && piece == Piece::WPAWN && pos.getWPawnFiles().exist(to.getFile())) {
			return;
		}
		// 打ち歩詰めチェック
		if (black && piece == Piece::BPAWN && pos.isPawnDropMate(to, true)) {
			return;
		} else if (!black && piece == Piece::WPAWN && pos.isPawnDropMate(to, false)) {
			return;
		}
		// 手追加
		moves[num++] = Move(Square::NON, to, false, true, piece);
		// 跳び駒
		if (longRange) {
			while (true) {
				to += dir;
				p = pos.getBoard(to);
				if (!p.isEmpty()) { break; }
				// 手追加
				moves[num++] = Move(Square::NON, to, false, true, piece);
			}
		}
	}

	int MoveGenerator::removeUnstable(const Move& move) {
		int removed = 0;
		for (int i = 0; i < (int)num; i++) {
			const Move& temp = get(i);
			if (temp == move) {
				removeUnstable(i);
				i--;
				removed++;
			}
		}
		return removed;
	}

}
