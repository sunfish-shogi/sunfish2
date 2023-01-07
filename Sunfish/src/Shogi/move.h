/*
 * move.h
 *
 *  Created on: 2012/05/10
 *      Author: ryosuke
 */

#ifndef MOVE_H_
#define MOVE_H_

#include "square.h"
#include "piece.h"
#include <iostream>

namespace Shogi {
	class Move {
	private:
		struct {
			unsigned from : 8;
			unsigned to : 8;
			unsigned promote : 1;
			unsigned hand : 1;
			unsigned piece : 5;
		} m;

	public:
		Move() {
			setEmpty();
		}

		Move(const Move& move) {
			m = move.m;
		}

		Move(const Square& from, const Square& to, bool promote,
				bool hand, const Piece& piece) {
			setFrom(from);
			setTo(to);
			setPromotion(promote);
			setHand(hand);
			setPiece(piece);
		}

		Move(unsigned um) {
			m.piece = (um >> 18);
			m.hand = (um >> 17) & 0x01;
			m.promote = (um >> 16) & 0x01;
			m.to = (um >> 8) & 0xff;
			m.from = um & 0xff;
		}

		void setEmpty() {
			m.to = 0;
		}

		bool isEmpty() const {
			return m.to == 0;
		}

		Square getFrom() const {
			return Square(m.from);
		}

		void setFrom(const Square& from)
		{
			m.from = from.getIndex();
		}

		Square getTo() const {
			return Square(m.to);
		}

		void setTo(const Square& to)
		{
			m.to = to.getIndex();
		}

		bool isHand() const {
			return m.hand;
		}

		void setHand(bool hand) {
			m.hand = hand;
		}

		bool isPromotion() const {
			return m.promote;
		}

		void setPromotion(bool promote) {
			m.promote = promote;
		}

		Piece getPiece() const {
			return Piece(m.piece);
		}

		void setPiece(const Piece& piece)
		{
			m.piece = piece.getInteger();
		}

		Move& operator=(const Move& move) {
			m = move.m;
			return *this;
		}

		bool operator<(const Move& move) const {
			return (unsigned)*this< (unsigned)move;
		}

		bool operator<=(const Move& move) const {
			return (unsigned)*this<= (unsigned)move;
		}

		bool operator>(const Move& move) const {
			return (unsigned)*this> (unsigned)move;
		}

		bool operator>=(const Move& move) const {
			return (unsigned)*this>= (unsigned)move;
		}

		bool operator==(const Move& move) const {
			return (unsigned)*this== (unsigned)move;
		}

		bool operator!=(const Move& move) const {
			return !operator==(move);
		}

		operator unsigned() const {
			return (m.piece << 18) | (m.hand << 17) | (m.promote << 16) | (m.to << 8) | m.from;
		}

		std::string toString() const;

		std::string toStringCsa() const;

		std::string toString4Debug() const;

		bool writeBinary(std::ostream& out) const {
			out.write((const char*)&m, sizeof(m));
			return true;
		}

		bool readBinary(std::istream& in) {
			in.read((char*)&m, sizeof(m));
			return !in.fail();
		}
	};
}

#endif /* MOVE_H_ */
