/*
 * feature.cpp
 *
 *  Created on: 2012/07/07
 *      Author: ryosuke
 */

#include "feature.h"
#include "../Tools/debug.h"

namespace Evaluates {
	using namespace Shogi;

	const int Feature::kppBlackHand[8] = {
		0,
		KPP_DAI_SFU,
		KPP_DAI_SKY,
		KPP_DAI_SKE,
		KPP_DAI_SGI,
		KPP_DAI_SKI,
		KPP_DAI_SKA,
		KPP_DAI_SHI,
	};

	const int Feature::kppWhiteHand[8] = {
		0,
		KPP_DAI_GFU,
		KPP_DAI_GKY,
		KPP_DAI_GKE,
		KPP_DAI_GGI,
		KPP_DAI_GKI,
		KPP_DAI_GKA,
		KPP_DAI_GHI,
	};

	const int Feature::kkpHand[8] = {
		0,
		KKP_DAI_FU,
		KKP_DAI_KY,
		KKP_DAI_KE,
		KKP_DAI_GI,
		KKP_DAI_KI,
		KKP_DAI_KA,
		KKP_DAI_HI,
	};

	// TODO: 使わない引数が残るのが嫌
	template<bool get, bool cum>
	Value Feature::extract(const Position& pos,
			const Param* pp, Gradient* pg, double inc) {
		Value ret = Value(0);
		int num = 0;
		int list0[64];
		int list1[64];
		int list2[64];

		Kings kings(pos);

		if (get) {
			ret += getKKP_H(*pp, kings, KKP_DAI_FU, pos.getBlackHand(Piece::PAWN), true);
			ret += getKKP_H(*pp, kings, KKP_DAI_FU, pos.getBlackHand(Piece::LANCE), true);
			ret += getKKP_H(*pp, kings, KKP_DAI_FU, pos.getBlackHand(Piece::KNIGHT), true);
			ret += getKKP_H(*pp, kings, KKP_DAI_FU, pos.getBlackHand(Piece::SILVER), true);
			ret += getKKP_H(*pp, kings, KKP_DAI_FU, pos.getBlackHand(Piece::GOLD), true);
			ret += getKKP_H(*pp, kings, KKP_DAI_FU, pos.getBlackHand(Piece::BISHOP), true);
			ret += getKKP_H(*pp, kings, KKP_DAI_FU, pos.getBlackHand(Piece::ROOK), true);
			ret += getKKP_H(*pp, kings, KKP_DAI_FU, pos.getWhiteHand(Piece::PAWN), false);
			ret += getKKP_H(*pp, kings, KKP_DAI_FU, pos.getWhiteHand(Piece::LANCE), false);
			ret += getKKP_H(*pp, kings, KKP_DAI_FU, pos.getWhiteHand(Piece::KNIGHT), false);
			ret += getKKP_H(*pp, kings, KKP_DAI_FU, pos.getWhiteHand(Piece::SILVER), false);
			ret += getKKP_H(*pp, kings, KKP_DAI_FU, pos.getWhiteHand(Piece::GOLD), false);
			ret += getKKP_H(*pp, kings, KKP_DAI_FU, pos.getWhiteHand(Piece::BISHOP), false);
			ret += getKKP_H(*pp, kings, KKP_DAI_FU, pos.getWhiteHand(Piece::ROOK), false);
		}
		if (cum) {
			addKKP_H(*pg, kings, KKP_DAI_FU, pos.getBlackHand(Piece::PAWN), true, inc);
			addKKP_H(*pg, kings, KKP_DAI_FU, pos.getBlackHand(Piece::LANCE), true, inc);
			addKKP_H(*pg, kings, KKP_DAI_FU, pos.getBlackHand(Piece::KNIGHT), true, inc);
			addKKP_H(*pg, kings, KKP_DAI_FU, pos.getBlackHand(Piece::SILVER), true, inc);
			addKKP_H(*pg, kings, KKP_DAI_FU, pos.getBlackHand(Piece::GOLD), true, inc);
			addKKP_H(*pg, kings, KKP_DAI_FU, pos.getBlackHand(Piece::BISHOP), true, inc);
			addKKP_H(*pg, kings, KKP_DAI_FU, pos.getBlackHand(Piece::ROOK), true, inc);
			addKKP_H(*pg, kings, KKP_DAI_FU, pos.getWhiteHand(Piece::PAWN), false, inc);
			addKKP_H(*pg, kings, KKP_DAI_FU, pos.getWhiteHand(Piece::LANCE), false, inc);
			addKKP_H(*pg, kings, KKP_DAI_FU, pos.getWhiteHand(Piece::KNIGHT), false, inc);
			addKKP_H(*pg, kings, KKP_DAI_FU, pos.getWhiteHand(Piece::SILVER), false, inc);
			addKKP_H(*pg, kings, KKP_DAI_FU, pos.getWhiteHand(Piece::GOLD), false, inc);
			addKKP_H(*pg, kings, KKP_DAI_FU, pos.getWhiteHand(Piece::BISHOP), false, inc);
			addKKP_H(*pg, kings, KKP_DAI_FU, pos.getWhiteHand(Piece::ROOK), false, inc);
		}

		for (Square square = Square::TOP; square.valid(); square.next()) {
			Piece piece = pos.getBoard(square);
			if (!piece.isEmpty()) {
				if (!piece.isKing()) {
					if (get) {
						ret += getKKP(*pp, kings, square, piece);
					}
					if (cum) {
						addKKP(*pg, kings, square, piece, inc);
					}
					list0[num] = getKPPIndex(square, piece, true);
					list2[num] = getKPPIndex(square, piece, false);
					num++;
				}
			}
		}

		for (int i = num - 1; i >= 0; i--) {
			list1[num-i-1] = list2[i];
		}

		num = listHand<false>(list0, list1, num, pos);

		// King Piece Piece
		for (int i = 0; i < num; i++) {
			int x = list0[i];
			int y = list1[i];
			for(int j = 0; j <= i; j++) {
				if (get) {
					ret += Value(pp->getKPP(
						kings.getBlack(), x, list0[j]));
					ret -= Value(pp->getKPP(
						kings.getBlackR(), y, list1[j]));
				}
				if (cum) {
					pg->addKPP(kings.getBlack(), x, list0[j], ValueF(inc));
					pg->addKPP(kings.getBlackR(), y, list1[j], ValueF(-inc));
				}
			}
		}
		return ret;
	}
	template Value Feature::extract<true, false>
			(const Position& pos,
			const Param* pp, Gradient* pg, double inc);
	template Value Feature::extract<false, true>
			(const Position& pos,
			const Param* pp, Gradient* pg, double inc);

	Value Feature::getDiff(const Position& pos, const Param& p,
			Shogi::Square from, Shogi::Square to,
			Shogi::Piece fromBefore,
			Shogi::Piece toBefore, Shogi::Piece toAfter) {
		Value ret = Value(0);

		Kings kings(pos);

		Piece handChange = Piece::EMPTY;

		int beforeNum = 0;
		int before0[8];
		int before1[8];
		int afterNum = 0;
		int after0[8];
		int after1[8];

		// 移動先の後の状態
		ret += getKKP(p, kings, to, toAfter);
		after0[afterNum] = getKPPIndex(to, toAfter, true);
		after1[afterNum] = getKPPIndex(to, toAfter, false);
		afterNum++;

		if (!fromBefore.isEmpty()) {
			// 移動元の前の状態
			assert(toAfter.isBlack() == fromBefore.isBlack());
			ret -= getKKP(p, kings, from, fromBefore);
			before0[beforeNum] = getKPPIndex(from, fromBefore, true);
			before1[beforeNum] = getKPPIndex(from, fromBefore, false);
			beforeNum++;

			if (!toBefore.isEmpty()) {
				// 移動先の前の状態 (取られた駒)
				assert(toAfter.isBlack() != toBefore.isBlack());
				ret -= getKKP(p, kings, to, toBefore);
				before0[beforeNum] = getKPPIndex(to, toBefore, true);
				before1[beforeNum] = getKPPIndex(to, toBefore, false);
				beforeNum++;

				// 持ち駒 (取った駒)
				Piece piece = toBefore.getUnpromoted().getTurned();
				handChange = piece;
				if (piece.isBlack()) {
					int handNum = pos.getBlackHand(piece);
					assert(handNum >= 1);
					ret -= getKKP_H(p, kings, kkpHand[piece], handNum - 1, true);
					before0[beforeNum] = kppBlackHand[piece] + handNum - 1;
					before1[beforeNum] = kppWhiteHand[piece] + handNum - 1;
					beforeNum++;
					ret += getKKP_H(p, kings, kkpHand[piece], handNum, true);
					after0[afterNum] = kppBlackHand[piece] + handNum;
					after1[afterNum] = kppWhiteHand[piece] + handNum;
					afterNum++;
				} else {
					piece.turn();
					int handNum = pos.getWhiteHand(piece);
					assert(handNum >= 1);
					ret -= getKKP_H(p, kings, kkpHand[piece], handNum - 1, false);
					before0[beforeNum] = kppWhiteHand[piece] + handNum - 1;
					before1[beforeNum] = kppBlackHand[piece] + handNum - 1;
					beforeNum++;
					ret += getKKP_H(p, kings, kkpHand[piece], handNum, false);
					after0[afterNum] = kppWhiteHand[piece] + handNum;
					after1[afterNum] = kppBlackHand[piece] + handNum;
					afterNum++;

				}
			}
		} else {
			// 持ち駒 (打った駒)
			Piece piece = toAfter;
			handChange = piece;
			if (piece.isBlack()) {
				int handNum = pos.getBlackHand(piece);
				ret -= getKKP_H(p, kings, kkpHand[piece], handNum + 1, true);
				before0[beforeNum] = kppBlackHand[piece] + handNum + 1;
				before1[beforeNum] = kppWhiteHand[piece] + handNum + 1;
				beforeNum++;
				ret += getKKP_H(p, kings, kkpHand[piece], handNum, true);
				after0[afterNum] = kppBlackHand[piece] + handNum;
				after1[afterNum] = kppWhiteHand[piece] + handNum;
				afterNum++;
			} else {
				piece.turn();
				int handNum = pos.getWhiteHand(piece);
				ret -= getKKP_H(p, kings, kkpHand[piece], handNum + 1, false);
				before0[beforeNum] = kppWhiteHand[piece] + handNum + 1;
				before1[beforeNum] = kppBlackHand[piece] + handNum + 1;
				beforeNum++;
				ret += getKKP_H(p, kings, kkpHand[piece], handNum, false);
				after0[afterNum] = kppWhiteHand[piece] + handNum;
				after1[afterNum] = kppBlackHand[piece] + handNum;
				afterNum++;
			}
		}

		int num = 0;
		int list0[64];
		int list1[64];
		int list2[64];

		for (Square square = Square::TOP; square.valid(); square.next()) {
			Piece piece = pos.getBoard(square);
			if (!piece.isEmpty()) {
				if (!piece.isKing()) {
					if (square == from && !fromBefore.isEmpty()) {
						continue;
					}
					if (square == to) {
						continue;
					}
					list0[num] = getKPPIndex(square, piece, true);
					list2[num] = getKPPIndex(square, piece, false);
					num++;
				}
			}
		}

		for (int i = num - 1; i >= 0; i--) {
			list1[num-i-1] = list2[i];
		}

		if (handChange != Piece::EMPTY) {
			num = listHand<true>(list0, list1, num, pos, handChange);
		} else {
			num = listHand<false>(list0, list1, num, pos);
		}

		ret -= getDiffKPP(p, kings.getBlack(),
				before0, beforeNum, list0, num);
		ret += getDiffKPP(p, kings.getBlackR(),
				before1, beforeNum, list1, num);

		ret += getDiffKPP(p, kings.getBlack(),
				after0, afterNum, list0, num);
		ret -= getDiffKPP(p, kings.getBlackR(),
				after1, afterNum, list1, num);

		return ret;
	}

	Value Feature::getDiffKPP(const Param& p, int king,
			const int changeList[], int changeNum,
			const int list[], int num) {
		Value ret = Value(0);
		for (int i = 0; i < changeNum; i++) {
			int index = changeList[i];
			for (int j = 0; j < i; j++) {
				assert(index != changeList[j]);
				ret += getDiffKPP(p, king, index, changeList[j]);
			}
			ret += Value(p.getKPP(king, index, index));
			for (int j = 0; j < num; j++) {
				assert(index != list[j]);
				ret += getDiffKPP(p, king, index, list[j]);
			}
		}
		return ret;
	}

	template <bool diff>
	int Feature::listHand(int list0[], int list1[], int num,
			const Position& pos, Piece change) {
		int num0 = num;
		int num1 = num;
		if (!diff || change != Piece::BPAWN) {
			list0[num0] = KPP_DAI_SFU + pos.getBlackHand(Piece::PAWN);
			num0++;
		}
		if (!diff || change != Piece::WPAWN) {
			list1[num1] = KPP_DAI_SFU + pos.getWhiteHand(Piece::PAWN);
			num1++;
		}
		if (!diff || change != Piece::BLANCE) {
			list0[num0] = KPP_DAI_SKY + pos.getBlackHand(Piece::LANCE);
			num0++;
		}
		if (!diff || change != Piece::WLANCE) {
			list1[num1] = KPP_DAI_SKY + pos.getWhiteHand(Piece::LANCE);
			num1++;
		}
		if (!diff || change != Piece::BKNIGHT) {
			list0[num0] = KPP_DAI_SKE + pos.getBlackHand(Piece::KNIGHT);
			num0++;
		}
		if (!diff || change != Piece::WKNIGHT) {
			list1[num1] = KPP_DAI_SKE + pos.getWhiteHand(Piece::KNIGHT);
			num1++;
		}
		if (!diff || change != Piece::BSILVER) {
			list0[num0] = KPP_DAI_SGI + pos.getBlackHand(Piece::SILVER);
			num0++;
		}
		if (!diff || change != Piece::WSILVER) {
			list1[num1] = KPP_DAI_SGI + pos.getWhiteHand(Piece::SILVER);
			num1++;
		}
		if (!diff || change != Piece::BGOLD) {
			list0[num0] = KPP_DAI_SKI + pos.getBlackHand(Piece::GOLD);
			num0++;
		}
		if (!diff || change != Piece::WGOLD) {
			list1[num1] = KPP_DAI_SKI + pos.getWhiteHand(Piece::GOLD);
			num1++;
		}
		if (!diff || change != Piece::BBISHOP) {
			list0[num0] = KPP_DAI_SKA + pos.getBlackHand(Piece::BISHOP);
			num0++;
		}
		if (!diff || change != Piece::WBISHOP) {
			list1[num1] = KPP_DAI_SKA + pos.getWhiteHand(Piece::BISHOP);
			num1++;
		}
		if (!diff || change != Piece::BROOK) {
			list0[num0] = KPP_DAI_SHI + pos.getBlackHand(Piece::ROOK);
			num0++;
		}
		if (!diff || change != Piece::WROOK) {
			list1[num1] = KPP_DAI_SHI + pos.getWhiteHand(Piece::ROOK);
			num1++;
		}

		if (!diff || change != Piece::WPAWN) {
			list0[num0] = KPP_DAI_GFU + pos.getWhiteHand(Piece::PAWN);
			num0++;
		}
		if (!diff || change != Piece::BPAWN) {
			list1[num1] = KPP_DAI_GFU + pos.getBlackHand(Piece::PAWN);
			num1++;
		}
		if (!diff || change != Piece::WLANCE) {
			list0[num0] = KPP_DAI_GKY + pos.getWhiteHand(Piece::LANCE);
			num0++;
		}
		if (!diff || change != Piece::BLANCE) {
			list1[num1] = KPP_DAI_GKY + pos.getBlackHand(Piece::LANCE);
			num1++;
		}
		if (!diff || change != Piece::WKNIGHT) {
			list0[num0] = KPP_DAI_GKE + pos.getWhiteHand(Piece::KNIGHT);
			num0++;
		}
		if (!diff || change != Piece::BKNIGHT) {
			list1[num1] = KPP_DAI_GKE + pos.getBlackHand(Piece::KNIGHT);
			num1++;
		}
		if (!diff || change != Piece::WSILVER) {
			list0[num0] = KPP_DAI_GGI + pos.getWhiteHand(Piece::SILVER);
			num0++;
		}
		if (!diff || change != Piece::BSILVER) {
			list1[num1] = KPP_DAI_GGI + pos.getBlackHand(Piece::SILVER);
			num1++;
		}
		if (!diff || change != Piece::WGOLD) {
			list0[num0] = KPP_DAI_GKI + pos.getWhiteHand(Piece::GOLD);
			num0++;
		}
		if (!diff || change != Piece::BGOLD) {
			list1[num1] = KPP_DAI_GKI + pos.getBlackHand(Piece::GOLD);
			num1++;
		}
		if (!diff || change != Piece::WBISHOP) {
			list0[num0] = KPP_DAI_GKA + pos.getWhiteHand(Piece::BISHOP);
			num0++;
		}
		if (!diff || change != Piece::BBISHOP) {
			list1[num1] = KPP_DAI_GKA + pos.getBlackHand(Piece::BISHOP);
			num1++;
		}
		if (!diff || change != Piece::WROOK) {
			list0[num0] = KPP_DAI_GHI + pos.getWhiteHand(Piece::ROOK);
			num0++;
		}
		if (!diff || change != Piece::BROOK) {
			list1[num1] = KPP_DAI_GHI + pos.getBlackHand(Piece::ROOK);
			num1++;
		}
		assert(num0 == num1);
		return num0;
	}

	Value Feature::estimate(const Shogi::Position& pos,
			const Param* pparam, const Shogi::Move& move) {
		Value value(0);
		if (!move.getPiece().isKing()) {
			Kings kings(pos);
			if (!move.isHand()) {
				value -= pparam->getKKP(kings,
					move.getPiece(), move.getFrom());
			}
			Shogi::Piece piece = move.isPromotion() ?
				move.getPiece().getPromoted() : move.getPiece();
			value += pparam->getKKP(kings, piece, move.getTo());
		}
		return value;
	}
}
