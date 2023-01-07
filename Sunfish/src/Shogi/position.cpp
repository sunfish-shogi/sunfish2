/*
 * position.cpp
 *
 *  Created on: 2012/05/13
 *      Author: ryosuke
 */

#include <cassert>
#include <sstream>
#include "../Csa/csa.h"
#include "position.h"
#include "../Evaluates/evaluate.h"
#include "squareDiff.h"

namespace Shogi {
	PositionHash* Position::pPositionHash = NULL;

	void Position::copy(const Position& position) {
		boardHash = position.boardHash;
		handHash = position.handHash;
		board.init(position.board);
		blackHand.init(position.blackHand);
		whiteHand.init(position.whiteHand);
		effectBoard.init(position.effectBoard);
		bking = position.bking;
		wking = position.wking;
		bpawns = position.bpawns;
		wpawns = position.wpawns;
		blackTurn = position.blackTurn;
	}

	std::string Position::toString() const {
		std::ostringstream oss;
		oss << "White:" << whiteHand.toString();
		oss << board.toString();
		oss << "Black:" << blackHand.toString();
		return oss.str();
	}

	std::string Position::toStringCsa() const {
		std::ostringstream oss;
		oss << board.toStringCsa();
		oss << Csa::CHAR_POS << Csa::CHAR_BLK << blackHand.toStringCsa();
		oss << Csa::CHAR_POS << Csa::CHAR_WHT << whiteHand.toStringCsa();
		oss << (blackTurn ? Csa::CHAR_BLK : Csa::CHAR_WHT) << '\n';
		return oss.str();
	}

	Util::uint64 Position::generateBoardHash() const {
		Util::uint64 _hash = U64(0);
		for (Square square = Square::TOP; square.valid(); square.next()) {
			Piece piece = board.get(square);
			if (!piece.isEmpty()) {
				_hash ^= hashBoard(piece, square);
			}
		}
		return _hash;
	}

	Util::uint64 Position::generateHandHash() const {
		Util::uint64 _hash = U64(0);
		for (Piece piece = Piece::PAWN; piece != Piece::KING; piece.toNext()) {
			for (int h = 0; h < blackHand.get(piece); h++) {
				_hash ^= hashHand(piece, h, true);
			}
			for (int h = 0; h < whiteHand.get(piece); h++) {
				_hash ^= hashHand(piece, h, false);
			}
		}
		return _hash;
	}

	template <bool black, bool quick>
	bool Position::isLegalMove(const Move& move) const {
		if (move.isEmpty()) {
			return false;
		}
		Piece piece = move.getPiece();
		if (!quick && black && !piece.isBlack()) {
			return false;
		}
		if (!quick && !black && !piece.isWhite()) {
			return false;
		}
		if (move.isHand()) { // 持ち駒を打つ場合
			if (!quick && (piece.getTurnedBlack() < Piece::BPAWN ||
					piece.getTurnedBlack() > Piece::BROOK)){
				return false; // 不正な駒番号
			}
			if (black && blackHand.get(piece) == 0) {
				return false; // 持ち駒が無い。
			}
			if (!black && whiteHand.get(piece) == 0) {
				return false; // 持ち駒が無い。
			}
			int file = move.getTo().getFile();
			if (black && piece == Piece::BPAWN && bpawns.exist(file)) {
				return false; // 二歩
			}
			if (!black && piece == Piece::WPAWN && wpawns.exist(file)) {
				return false; // 二歩
			}
			if (!quick && move.getTo().isCompulsoryPromotion(piece)) {
				return false; // 行きどころのない駒
			}
			Piece pieceTo = board.get(move.getTo());
			if (!pieceTo.isEmpty()) {
				return false; // 移動先に駒がある。
			}
			if (isCheck()) { // 王手がかかっている場合
				DirectionFlags flags = getCheckDirection();
				if (flags.isPlural()) {
					return false; // 両王手
				}
				SquareDiff diff(move.getTo(), black? bking : wking);
				if (flags.toDirection() != diff.toDirection()){
					return false; // 王手している方向ではない。
				}
				if (!effectBoard.get(move.getTo(), black).king()) {
					return false; // 王手している駒より遠い。
				}
			}
			if (black && piece == Piece::BPAWN && isPawnDropMate(move.getTo(), true)) {
				return false;
			} else if (!black && piece == Piece::WPAWN && isPawnDropMate(move.getTo(), false)) {
				return false;
			}
			return true;
		} else { // 盤上の駒を動かす場合
			if (piece != getBoard(move.getFrom())) {
				return false; // 不正な駒
			}
			if (!quick && move.isPromotion() && (!piece.isPromotable() || 
					(!move.getTo().isPromotableRank(black) && !move.getFrom().isPromotableRank(black)))) {
				return false; // 成れない駒
			}
			if (!quick && !move.isPromotion() && move.getTo().isCompulsoryPromotion(piece)) {
				return false; // 行きどころのない駒
			}
			SquareDiff diff = SquareDiff(move.getFrom(), move.getTo());
			DirectionAndRange dir = diff.toDirectionAndRange();
			if (dir == Direction::NON) {
				return false; // 移動不能
			}
			if ((black && move.getPiece() == Piece::BKING) || // 先手玉
					(!black && move.getPiece() == Piece::WKING)) { // 後手玉
				DirectionFlags effectTo = effectBoard.get(move.getTo(), !black);
				DirectionFlags effectFrom = effectBoard.get(move.getFrom(), !black);
				if (effectFrom.getLongRangeOnly().check(dir, true) // 利きの方向に移動しようとしている。
						|| effectTo.longOrShortRange()) { // 移動先に利きがある。
					return false;
				}
			} else { // 玉以外
				if (isCheck()) {
					DirectionFlags flags = getCheckDirection();
					if (flags.isPlural()) {
						return false; // 両王手
					}
					SquareDiff diff(move.getTo(), black? bking : wking);
					if (flags.toDirection() != diff.toDirection()){
						return false; // 王手している方向ではない。
					}
					if (flags.shortRange()) {
						if (move.getTo() != (black ? bking : wking) - flags.toDirection()) {
							return false;
						}
					} else {
						if (!effectBoard.get(move.getTo(), black).king()) {
							return false; // 王手している駒より遠い。
						}
					}
				}
				Direction p = pin(move.getFrom(), black).toDirection();
				if (p != Direction::NON && dir != p && dir != p.reverse()) {
					return false;
				}
			}
			if (dir.isShortRange()) { // 1マス
				// 移動可能な方向か
				if (piece.getMovableDirection().check(dir) || piece.getMovableDirection().check(dir, true)) {
					Piece pieceTo = board.get(move.getTo());
					// 移動先に駒がいないか相手の駒か
					return pieceTo.isEmpty() || (black ? pieceTo.isWhite() : pieceTo.isBlack());
				}
				return false;
			} else { // 跳び駒の移動
				// 移動可能な方向か
				if (piece.getMovableDirection().check(dir)) {
					// 間に駒がないか
					for (Square sq = move.getFrom() + dir; ; sq += dir) {
						Piece pieceTo = board.get(sq);
						if (!pieceTo.isEmpty()) {
							return sq == move.getTo() && (black ? pieceTo.isWhite() : pieceTo.isBlack());
						}
						if (sq == move.getTo()) {
							return true;
						}
					}
				}
				return false;
			}
		}
	}
	template bool Position::isLegalMove<true, true>(const Move& move) const;
	template bool Position::isLegalMove<false, true>(const Move& move) const;
	template bool Position::isLegalMove<true, false>(const Move& move) const;
	template bool Position::isLegalMove<false, false>(const Move& move) const;

	template <bool black, bool chNotNull, bool evNotNull>
	void Position::moveUnsafe(const Move& move, Change* change,
			Evaluates::Evaluate* eval) {
#ifndef NDEBUG
		if (!isLegalMove<black, false>(move)) {
			Log::debug << "ERROR " << __FILE__ << '(' << __LINE__ << ") : ILLEGAL MOVE!!\n";
			Log::debug << toString();
			Log::debug << move.toString() << '\n';
			abort();
		}
#endif
		if (chNotNull) {
			change->setBoardHash(boardHash); // board hash
			change->setHandHash(handHash); // hand hash
			change->setBlackKing(bking); // black king's square
			change->setWhiteKing(wking); // white king's square
			change->setBlackPawns(bpawns); // black pawns
			change->setWhitePawns(wpawns); // white pawns
		}
		if (move.isHand()) { // 持ち駒を打つ場合
			if (chNotNull) { change->setType(Change::DROP); } // type of change
			Piece piece = move.getPiece();
			if (black) { // 先手
				assert(piece.isBlack());
				if (chNotNull) { change->setHandNum(blackHand.get(piece)); } // number of pieces
				if (chNotNull) { change->setHandPiece(piece); } // dropping piece
				if (chNotNull) { change->setToSquare(move.getTo()); } // dropping square
				blackHand.dec(piece); // hand
				handHash ^= hashHand(piece, blackHand.get(piece), true); // hand hash
				board.set(move.getTo(), piece); // board
				boardHash ^= hashBoard(piece, move.getTo()); // board hash
				if (piece == Piece::BPAWN) {
					bpawns.set(move.getTo().getFile());
				}
				effectBoard.change<true, true>(move.getTo(), piece.getMovableDirection(), board); // effect
			} else { // 後手
				assert(piece.isWhite());
				if (chNotNull) { change->setHandNum(whiteHand.get(piece)); } // number of pieces
				if (chNotNull) { change->setHandPiece(piece); } // dropping piece
				if (chNotNull) { change->setToSquare(move.getTo()); } // dropping square
				whiteHand.dec(piece); // hand
				handHash ^= hashHand(piece, whiteHand.get(piece), false); // hand hash
				board.set(move.getTo(), piece); // board
				boardHash ^= hashBoard(piece, move.getTo()); // board hash
				if (piece == Piece::WPAWN) {
					wpawns.set(move.getTo().getFile());
				}
				effectBoard.change<false, true>(move.getTo(), piece.getMovableDirection(), board); // effect
			}
			if (evNotNull) {
				eval->updatePositionalValue(*this,
						move.getTo(), move.getPiece());
			}
		} else { // 盤上の駒
			Piece piece = board.set(move.getFrom(), Piece::EMPTY); // board
			Piece fromBefore = piece;
			boardHash ^= hashBoard(piece, move.getFrom()); // board hash
			if (chNotNull) { change->setFromSquare(move.getFrom()); } // move from
			if (chNotNull) { change->setFromPiece(piece); } // moved piece
			if (black) { // 先手
				effectBoard.change<true, false>(move.getFrom(), piece.getMovableDirection(), board); // effect
			} else { // 後手
				effectBoard.change<false, false>(move.getFrom(), piece.getMovableDirection(), board); // effect
			}
			if (move.isPromotion()) { // 成
				if (black && piece == Piece::BPAWN) { // 先手の歩
					bpawns.unset(move.getFrom().getFile());
				} else if (!black && piece == Piece::WPAWN) { // 後手の歩
					wpawns.unset(move.getFrom().getFile());
				}
				if (evNotNull) { eval->addBaseValuePr(piece); } // evaluate
				piece.promote();
			} else if (black && piece.isKing<true>()) { // 先手玉
				bking = move.getTo();
			} else if (!black && piece.isKing<false>()) { // 後手玉
				wking = move.getTo();
			}
			Piece capture = board.set(move.getTo(), piece); // board
			boardHash ^= hashBoard(piece, move.getTo()); // board hash
			if (chNotNull) { change->setToSquare(move.getTo()); } // move to
			if (chNotNull) { change->setToPiece(capture); } // captured piece
			assert(capture != Piece::BKING);
			assert(capture != Piece::WKING);
			if (!capture.isEmpty()) {
				assert(capture != Piece::BKING);
				assert(capture != Piece::WKING);
				boardHash ^= hashBoard(capture, move.getTo()); // board hash
				if (chNotNull) { change->setType(Change::CAPTURE); } // type of change
				if (evNotNull) { eval->subBaseValueEx(capture); } // evaluate
				Piece captureUP = capture.getUnpromoted();
				if (black) {
					if (chNotNull) { change->setHandNum(blackHand.get(captureUP)); } // number of pieces
					if (chNotNull) { change->setHandPiece(captureUP); } // captured piece
					handHash ^= hashHand(captureUP, blackHand.get(captureUP), true); // hand hash
					blackHand.inc(captureUP); // hand
					effectBoard.change<false, false>(move.getTo(), capture.getMovableDirection(), board); // effect
					if (capture == Piece::WPAWN) { // 後手の歩
						wpawns.unset(move.getTo().getFile());
					}
				} else {
					if (chNotNull) { change->setHandNum(whiteHand.get(captureUP)); } // number of pieces
					if (chNotNull) { change->setHandPiece(captureUP); } // captured piece
					handHash ^= hashHand(captureUP, whiteHand.get(captureUP), false); // hand hash
					whiteHand.inc(captureUP); // hand
					effectBoard.change<true, false>(move.getTo(), capture.getMovableDirection(), board); // effect
					if (capture == Piece::BPAWN) { // 先手の歩
						bpawns.unset(move.getTo().getFile());
					}
				}
			} else {
				if (chNotNull) { change->setType(Change::NO_CAPTURE); } // type of change
			}
			if (black) {
				effectBoard.change<true, true>(move.getTo(), piece.getMovableDirection(), board);
			} else {
				effectBoard.change<false, true>(move.getTo(), piece.getMovableDirection(), board);
			}
			if (evNotNull) {
				eval->updatePositionalValue(*this,
						move.getFrom(), move.getTo(),
						fromBefore, capture, piece);
			}
		}
		turn();
	}
	template void Position::moveUnsafe<true, true, true>
			(const Move& move, Change* change,
			Evaluates::Evaluate* eval);
	template void Position::moveUnsafe<true, false, true>
			(const Move& move, Change* change,
			Evaluates::Evaluate* eval);
	template void Position::moveUnsafe<false, true, true>
			(const Move& move, Change* change,
			Evaluates::Evaluate* eval);
	template void Position::moveUnsafe<false, false, true>
			(const Move& move, Change* change,
			Evaluates::Evaluate* eval);
	template void Position::moveUnsafe<true, true, false>
			(const Move& move, Change* change,
			Evaluates::Evaluate* eval);
	template void Position::moveUnsafe<true, false, false>
			(const Move& move, Change* change,
			Evaluates::Evaluate* eval);
	template void Position::moveUnsafe<false, true, false>
			(const Move& move, Change* change,
			Evaluates::Evaluate* eval);
	template void Position::moveUnsafe<false, false, false>
			(const Move& move, Change* change,
			Evaluates::Evaluate* eval);

	template <bool chNotNull>
	bool Position::nullMove(Change* change) {
		if (isCheck()) {
			return false;
		}
		if (chNotNull) {
			change->setType(Change::NULL_MOVE); // type of change
		}
		turn();
		return true;
	}
	template bool Position::nullMove<true>(Change* change);
	template bool Position::nullMove<false>(Change* change);

	template <bool black>
	void Position::back(const Change& change) {
		Piece handPiece;
		Piece fromPiece;
		Piece toPiece;
		Piece toAfterPiece;
		Square to;
		Square from;
		switch (change.getType()) {
		case Change::NULL_MOVE:
			turn();
			return;
		case Change::DROP:
			handPiece = change.getHandPiece();
			to = change.getToSquare();
			if (black) {
				blackHand.set(handPiece, change.getHandNum());
			} else {
				whiteHand.set(handPiece, change.getHandNum());
			}
			board.set(to, Piece::EMPTY);
			if (black) {
				effectBoard.change<true, false>(to, handPiece.getMovableDirection(), board); // effect
			} else {
				effectBoard.change<false, false>(to, handPiece.getTurnedWhite().getMovableDirection(), board); // effect
			}
			break;
		case Change::CAPTURE:
			from = change.getFromSquare();
			to = change.getToSquare();
			fromPiece = change.getFromPiece();
			toPiece = change.getToPiece();
			board.set(from, fromPiece);
			if (black) { // 先手
				effectBoard.change<true, true>(from, fromPiece.getMovableDirection(), board); // effect
			} else { // 後手
				effectBoard.change<false, true>(from, fromPiece.getMovableDirection(), board); // effect
			}
			toAfterPiece = board.set(to, toPiece);
			if (black) {
				effectBoard.change<true, false>(to, toAfterPiece.getMovableDirection(), board); // effect
			} else {
				effectBoard.change<false, false>(to, toAfterPiece.getMovableDirection(), board); // effect
			}
			handPiece = change.getHandPiece();
			if (black) {
				blackHand.set(handPiece, change.getHandNum());
				effectBoard.change<false, true>(to, toPiece.getMovableDirection(), board); // effect
			} else {
				whiteHand.set(handPiece, change.getHandNum());
				effectBoard.change<true, true>(to, toPiece.getMovableDirection(), board); // effect
			}
			break;
		case Change::NO_CAPTURE:
			from = change.getFromSquare();
			to = change.getToSquare();
			fromPiece = change.getFromPiece();
			toPiece = change.getToPiece();
			board.set(from, fromPiece);
			if (black) { // 先手
				effectBoard.change<true, true>(from, fromPiece.getMovableDirection(), board); // effect
			} else { // 後手
				effectBoard.change<false, true>(from, fromPiece.getMovableDirection(), board); // effect
			}
			toAfterPiece = board.set(to, toPiece);
			if (black) {
				effectBoard.change<true, false>(to, toAfterPiece.getMovableDirection(), board); // effect
			} else {
				effectBoard.change<false, false>(to, toAfterPiece.getMovableDirection(), board); // effect
			}
			break;
		}
		turn();
		boardHash = change.getBoardHash();
		handHash = change.getHandHash();
		bking = change.getBlackKing();
		wking = change.getWhiteKing();
		bpawns = change.getBlackPawns();
		wpawns = change.getWhitePawns();
	}
	template void Position::back<true>(const Change& change);
	template void Position::back<false>(const Change& change);

	template <bool black, unsigned excludingFlag>
	bool Position::isKingMovable(Direction dir) const {
		Square to = (black ? bking : wking) + dir; // 移動先
		Piece piece = board.get(to); // 移動先のマス
		if (piece.isWall()) {
			return false; // 盤外
		}
		if (!piece.isEmpty()) {
			// 移動先に自分の駒があれば移動不可
			if (black && piece.isBlack()) {
				return false;
			} else if (!black && piece.isWhite()) {
				return false;
			}
		}
		// 移動先の相手の利き
		DirectionFlags flags = effectBoard.get(to, !black);
		// 打った駒によって消える利きは除外
		flags.remove(DirectionFlags(excludingFlag));
		return !flags.longOrShortRange();
	}

	// 歩打ちに対して玉の移動以外で王手回避可能か
	template<bool black>
	bool Position::isEvadablePawn() const {
		Square king = black ? wking : bking;
		Square square = king + (black ? Direction::DOWN : Direction::UP);
		DirectionFlags flags = effectBoard.get(square, !black).getExcludeKing();
		flags.remove(DirectionFlags(black
				? DirectionFlags::SHORT_DOWN
				: DirectionFlags::SHORT_UP));
		while (flags.isNonZero()) {
			Direction dir = flags.pop().toDirection().reverse();
			Square from = square;
			for (from += dir; getBoard(from) == Piece::EMPTY; from += dir)
				;
			if (pin(from, !black).isSubsetOf(
					DirectionFlags(black
					? DirectionFlags::LONG_UP
					: DirectionFlags::LONG_DOWN))) {
				return true;
			}
		}
		return false;
	}

	template<bool black>
	bool Position::canPawnDropCheck() const {
		if (isEvadablePawn<black>()) {
			return true;
		}
		if (black) {
			return isKingMovable<false, DirectionFlags::NON           >(Direction::LEFT_UP)
				|| isKingMovable<false, DirectionFlags::NON           >(Direction::UP)
				|| isKingMovable<false, DirectionFlags::NON           >(Direction::RIGHT_UP)
				|| isKingMovable<false, DirectionFlags::LONG_LEFT_UP  >(Direction::LEFT)
				|| isKingMovable<false, DirectionFlags::LONG_RIGHT_UP >(Direction::RIGHT)
				|| isKingMovable<false, DirectionFlags::LONG_LEFT     >(Direction::LEFT_DOWN)
				|| isKingMovable<false, DirectionFlags::NON           >(Direction::DOWN)
				|| isKingMovable<false, DirectionFlags::LONG_RIGHT    >(Direction::RIGHT_DOWN);
		} else {
			return isKingMovable<true, DirectionFlags::LONG_LEFT      >(Direction::LEFT_UP)
				|| isKingMovable<true, DirectionFlags::NON            >(Direction::UP)
				|| isKingMovable<true, DirectionFlags::LONG_RIGHT     >(Direction::RIGHT_UP)
				|| isKingMovable<true, DirectionFlags::LONG_LEFT_DOWN >(Direction::LEFT)
				|| isKingMovable<true, DirectionFlags::LONG_RIGHT_DOWN>(Direction::RIGHT)
				|| isKingMovable<true, DirectionFlags::NON            >(Direction::LEFT_DOWN)
				|| isKingMovable<true, DirectionFlags::NON            >(Direction::DOWN)
				|| isKingMovable<true, DirectionFlags::NON            >(Direction::RIGHT_DOWN);
		}
	}
	template bool Position::canPawnDropCheck<true>() const;
	template bool Position::canPawnDropCheck<false>() const;

	template<bool black>
	bool Position::isEvadable(const Square square, const DirectionFlags except) const {
		DirectionFlags flags = effectBoard.get(square, black).getExcludeKing();
		flags.remove(except);
		while (flags.isNonZero()) {
			Direction dir = flags.pop().toDirection().reverse();
			Square from = square;
			for (from += dir; getBoard(from) == Piece::EMPTY; from += dir)
				;
			if (pin(from, black).isSubsetOf(
					DirectionFlags(black
					? DirectionFlags::LONG_DOWN
					: DirectionFlags::LONG_UP))) {
				return true;
			}
		}
		return false;
	}

	template <bool black>
	bool Position::_isMate() const {
		DirectionFlags flags = getCheckDirection();
		if (flags.isZero()) {
			return false;
		}
		// 玉の移動
		if (isKingMovable<black>(Direction::LEFT_UP, flags) ||
				isKingMovable<black>(Direction::UP, flags) ||
				isKingMovable<black>(Direction::RIGHT_UP, flags) ||
				isKingMovable<black>(Direction::LEFT, flags) ||
				isKingMovable<black>(Direction::RIGHT, flags) ||
				isKingMovable<black>(Direction::LEFT_DOWN, flags) ||
				isKingMovable<black>(Direction::DOWN, flags) ||
				isKingMovable<black>(Direction::RIGHT_DOWN, flags)
				) {
			return false;
		}
		if (!flags.isPlural()) { // 両王手でない場合
			Square sq = (black ? bking : wking);
			Direction dir = flags.toDirection().reverse();
			// 距離が1出ない場合は合駒ができれば不詰め
			sq += dir;
			if (getBoard(sq) == Piece::EMPTY) {
				// どこにでも打てる持ち駒
				if (black) {
					if (getBlackHand(Piece::SILVER != 0) ||
							getBlackHand(Piece::GOLD != 0) ||
							getBlackHand(Piece::BISHOP != 0) ||
							getBlackHand(Piece::ROOK) != 0) {
						return false;
					}
				} else {
					if (getWhiteHand(Piece::SILVER) != 0 ||
							getWhiteHand(Piece::GOLD) != 0 ||
							getWhiteHand(Piece::BISHOP) != 0 ||
							getWhiteHand(Piece::ROOK) != 0) {
						return false;
					}
				}
				do {
					// 持ち駒
					if (black &&
							(isDropable(sq, Piece::BPAWN)
							|| isDropable(sq, Piece::BLANCE)
							|| isDropable(sq, Piece::BKNIGHT)
							)) {
						return false;
					} else if (!black &&
							(isDropable(sq, Piece::WPAWN)
							|| isDropable(sq, Piece::WLANCE)
							|| isDropable(sq, Piece::WKNIGHT)
							)) {
						return false;
					}
					// 移動合
					if (isEvadable<black>(sq, DirectionFlags(dir))) {
						return false;
					}
					sq += dir;
				} while (getBoard(sq) != Piece::EMPTY);
			}
			// 王手している駒を取る手
			if (isEvadable<black>(sq, DirectionFlags(dir))) {
				return false;
			}
		}
		return true;
	}
	template bool Position::_isMate<true>() const;
	template bool Position::_isMate<false>() const;
}

