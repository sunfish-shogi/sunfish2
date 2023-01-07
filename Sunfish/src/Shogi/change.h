/*
 * change.cpp
 *
 *  Created on: 2012/06/16
 *      Author: ryosuke
 */

#ifndef CHANGE_H_
#define CHANGE_H_

#include "../Util/int.h"
#include "square.h"
#include "piece.h"

namespace Shogi {
	class Change {
	private:
		struct {
			Util::uint64 boardHash;
			Util::uint64 handHash;
			unsigned type : 2;
			unsigned handPiece : 3;
			unsigned handNum : 5;
			unsigned fromSquare : 8;
			unsigned fromPiece : 5;
			unsigned toSquare : 8;
			unsigned toPiece : 5;
			unsigned bking : 8;
			unsigned wking : 8;
			unsigned bpawns : 10;
			unsigned wpawns : 10;
		} c;

	public:
		enum {
			NULL_MOVE = 0,
			DROP,
			NO_CAPTURE,
			CAPTURE,
		};

		Change() {
		}

		Change(const Change& change) {
			c = change.c;
		}

		unsigned getType() const {
			return c.type;
		}

		void setType(unsigned type) {
			c.type = type;
		}

		Piece getHandPiece() const {
			return Piece(c.handPiece);
		}

		void setHandPiece(const Piece& piece) {
			c.handPiece = piece.getTurnedBlack().getInteger();
		}

		unsigned getHandNum() const {
			return c.handNum;
		}

		void setHandNum(int num) {
			c.handNum = num;
		}

		Square getFromSquare() const {
			return Square(c.fromSquare);
		}

		void setFromSquare(const Square& square) {
			c.fromSquare = square.getIndex();
		}

		Piece getFromPiece() const {
			return Piece(c.fromPiece);
		}

		void setFromPiece(const Piece& piece) {
			c.fromPiece = piece.getInteger();
		}

		Square getToSquare() const {
			return Square(c.toSquare);
		}

		void setToSquare(const Square& square) {
			c.toSquare = square.getIndex();
		}

		Piece getToPiece() const {
			return Piece(c.toPiece);
		}

		void setToPiece(const Piece& piece) {
			c.toPiece = piece.getInteger();
		}

		Util::uint64 getBoardHash() const {
			return c.boardHash;
		}

		void setBoardHash(Util::uint64 boardHash) {
			c.boardHash = boardHash;
		}

		Util::uint64 getHandHash() const {
			return c.handHash;
		}

		void setHandHash(Util::uint64 handHash) {
			c.handHash = handHash;
		}

		Square getBlackKing() const {
			return Square(c.bking);
		}

		void setBlackKing(const Square& square) {
			c.bking = square.getIndex();
		}

		Square getWhiteKing() const {
			return Square(c.wking);
		}

		void setWhiteKing(const Square& square) {
			c.wking = square.getIndex();
		}

		PawnFlags getBlackPawns() const {
			PawnFlags pawns;
			pawns.setBits(c.bpawns);
			return pawns;
		}

		void setBlackPawns(const PawnFlags& pawns) {
			c.bpawns = pawns.getBits();
		}

		PawnFlags getWhitePawns() const {
			PawnFlags pawns;
			pawns.setBits(c.wpawns);
			return pawns;
		}

		void setWhitePawns(const PawnFlags& pawns) {
			c.wpawns = pawns.getBits();
		}

		std::string toString() const {
			std::ostringstream oss;
			oss << "boardHash=[" << c.boardHash << "], ";
			oss << "handHash=[" << c.handHash << "], ";
			oss << "type=[" << c.type << "], ";
			oss << "handPiece=[" << c.handPiece << "], ";
			oss << "handNum=[" << c.handNum << "], ";
			oss << "fromSquare=[" << c.fromSquare << "], ";
			oss << "fromPiece=[" << c.fromPiece << "], ";
			oss << "toSquare=[" << c.toSquare << "], ";
			oss << "toPiece=[" << c.toPiece << "]";
			return oss.str();
		}
	};
}

#endif // CHANGE_H_
