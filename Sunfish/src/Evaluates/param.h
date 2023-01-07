/*
 * param.h
 *
 *  Created on: 2012/07/01
 *      Author: ryosuke
 */

#ifndef PARAM_H_
#define PARAM_H_

#include "../Shogi/position.h"
#include "value.h"

namespace Evaluates {
	extern const int sym[];

	extern const int blackPiece[];
	extern const int whitePiece[];

	class Kings {
	private:
		int bking;
		int wking;
		int bking2;
		int wking2;

	public:
		Kings(const Shogi::Position& pos) {
			bking = pos.getBKing().getShortIndex();
			wking = pos.getWKing().getShortIndex();
			bking2 = 80 - wking;
			wking2 = 80 - bking;
		}

		int getBlack() const { return bking; }
		int getWhite() const { return wking; }
		int getBlackR() const { return bking2; }
		int getWhiteR() const { return wking2; }
	};


	enum{
		// King Piece Piece
		KPP_KNUM    = 18,
	
		KPP_BAN = 0,
		KPP_DAI_SFU = KPP_BAN     + 81 * KPP_KNUM,
		KPP_DAI_SKY = KPP_DAI_SFU + 19,
		KPP_DAI_SKE = KPP_DAI_SKY + 5,
		KPP_DAI_SGI = KPP_DAI_SKE + 5,
		KPP_DAI_SKI = KPP_DAI_SGI + 5,
		KPP_DAI_SKA = KPP_DAI_SKI + 5,
		KPP_DAI_SHI = KPP_DAI_SKA + 3,
	
		KPP_DAI_GFU = KPP_DAI_SHI + 3,
		KPP_DAI_GKY = KPP_DAI_GFU + 19,
		KPP_DAI_GKE = KPP_DAI_GKY + 5,
		KPP_DAI_GGI = KPP_DAI_GKE + 5,
		KPP_DAI_GKI = KPP_DAI_GGI + 5,
		KPP_DAI_GKA = KPP_DAI_GKI + 5,
		KPP_DAI_GHI = KPP_DAI_GKA + 3,
	
		KPP_MAX     = KPP_DAI_GHI + 3,
		KPP_SIZE    = KPP_MAX*(KPP_MAX+1)/2,
		KPP_ALL     = 81 * KPP_SIZE,
	
		// King King Piece
		KKP_KNUM    = 9,
	
		KKP_BAN     = 0,
	
		KKP_DAI_FU  = KKP_BAN    + 81 * KKP_KNUM,
		KKP_DAI_KY  = KKP_DAI_FU + 19,
		KKP_DAI_KE  = KKP_DAI_KY + 5,
		KKP_DAI_GI  = KKP_DAI_KE + 5,
		KKP_DAI_KI  = KKP_DAI_GI + 5,
		KKP_DAI_KA  = KKP_DAI_KI + 5,
		KKP_DAI_HI  = KKP_DAI_KA + 3,
	
		KKP_MAX     = KKP_DAI_HI + 3,
		KKP_ALL     = 81 * 81 * KKP_MAX,
	};

	template <class T, class U>
	class TempParam {
	private:
		typedef TempParam<T, U> Type;

		T piece[Shogi::Piece::DRAGON+1];
		T pieceEx[Shogi::Piece::DRAGON+1];
		T piecePr[Shogi::Piece::DRAGON+1];

		// King Piece Piece
		U kpp[81][KPP_SIZE];

		// King King Piece
		U kkp[81][81][KKP_MAX];

		template <bool copy>
		void symmetry() {
			int x0, y0, z0;
			int x1, y1, z1;

			for (x0 = 0; x0 < 81; x0++) {
				x1 = sym[x0]; // 左右反転
				if( x0 > x1 ) { continue; }

				// King Piece Piece
				for (y0 = 0; y0 < KPP_MAX; y0++) {
					if( y0 < KPP_DAI_SFU ){
						// 盤上は左右反転
						y1 = sym[y0/KPP_KNUM] * KPP_KNUM + ( y0 % KPP_KNUM );
						if( x0 == x1 && y0 > y1 ) { continue; }
					}
					else{ y1 = y0; } // 駒台はそのまま
					for (z0 = 0; z0 <= y0; z0++) {
						if( z0 < KPP_DAI_SFU ){
							// 盤上は左右反転
							z1 = sym[z0/KPP_KNUM] * KPP_KNUM + ( z0 % KPP_KNUM );
							if( x0 == x1 && y0 == y1 && z0 >= z1 ) {
								continue;
							}
						}
						else{ z1 = z0; } // 駒台はそのまま
						if( y1 >= z1 ) {
							int i0 = y0*(y0+1)/2+z0;
							int i1 = y1*(y1+1)/2+z1;
							if (copy) {
								kpp[x0][i0] = kpp[x1][i1];
							} else {
								kpp[x0][i0] = kpp[x1][i1] = kpp[x0][i0] + kpp[x1][i1];
							}
						} else {
							int i0 = y0*(y0+1)/2+z0;
							int i1 = z1*(z1+1)/2+y1;
							if (copy) {
								kpp[x0][i0] = kpp[x1][i1];
							} else {
								kpp[x0][i0] = kpp[x1][i1] = kpp[x0][i0] + kpp[x1][i1];
							}
						}
					}
				}

				// King King Piece
				for (y0 = 0; y0 < 81; y0++) {
					if( y0 == x0 ) { continue; }
					y1 = sym[y0];
					if( x0 == x1 && y0 > y1 ) { continue; }
					for (z0 = 0; z0 < KKP_MAX; z0++) {
						if( z0 < KKP_DAI_FU ){
							// 盤上は左右反転
							z1 = sym[z0/KKP_KNUM] * KKP_KNUM + ( z0 % KKP_KNUM );
							if( x0 == x1 && y0 == y1 && z0 >= z1 ) {
								continue;
							}
						}
						else{
							// 駒台はそのまま
							if( x0 == x1 && y0 == y1 ) {
								continue;
							}
							z1 = z0;
						}
						if (copy) {
							kkp[x0][y0][z0] = kkp[x1][y1][z1];
						} else {
							kkp[x0][y0][z0] = kkp[x1][y1][z1] = kkp[x0][y0][z0] + kkp[x1][y1][z1];
						}
					}
				}
			}
		}

	public:
		static const int SCALE = 32;

		TempParam(const char* filename = NULL) {
			init();
			if (filename != NULL) {
				read(filename);
			}
		}

		virtual ~TempParam() {
		}

		void init();

		bool read(const char* filename);

		bool write(const char* filename);

		void cumulate();

		void decumulate();

		void symCopy() {
			symmetry<true>();
		}

		void symAdd(){
			symmetry<false>();
		}

		void setPiece(const Shogi::Piece& p, T value) {
			Shogi::Piece bp = p.getTurnedBlack();
			piece[bp] = value;
		}

		void addPiece(const Shogi::Piece& p, T value) {
			Shogi::Piece bp = p.getTurnedBlack();
			piece[bp] += value;
		}

		T getPiece(const Shogi::Piece& p) const {
			if (!p.isWhite()) {
				return  piece[p];
			} else {
				return -piece[p.getTurnedBlack()];
			}
		}

		T getPieceAbs(const Shogi::Piece& p) const {
			return piece[p.getTurnedBlack()];
		}

		T getPieceExchange(const Shogi::Piece& p) const {
			if (!p.isWhite()) {
				return  pieceEx[p];
			} else {
				return -pieceEx[p.getTurnedBlack()];
			}
		}

		T getPieceExchangeAbs(const Shogi::Piece& p) const {
			return pieceEx[p.getTurnedBlack()];
		}

		T getPiecePromote(const Shogi::Piece& p) const {
			if (!p.isWhite()) {
				return  piecePr[p];
			} else {
				return -piecePr[p.getTurnedBlack()];
			}
		}

		T getPiecePromoteAbs(const Shogi::Piece& p) const {
			return piecePr[p.getTurnedBlack()];
		}

		// index1 >= index2
		static int getKPPIndex(int index1, int index2) {
			return index1 * (index1 + 1) / 2 + index2;
		}

		// index1 >= index2
		void setKPP(int kingIndex, int index1, int index2, const U& value) {
			kpp[kingIndex][getKPPIndex(index1, index2)] = value;
		}

		void setKPP(int kingIndex, int pieceIndex, const U& value) {
			kpp[kingIndex][pieceIndex] = value;
		}

		void setKPP(int index, const U& value) {
			kpp[0][index] = value;
		}

		// index1 >= index2
		void addKPP(int kingIndex, int index1, int index2, const U& value) {
			kpp[kingIndex][getKPPIndex(index1, index2)] += value;
		}

		void addKPP(int kingIndex, int pieceIndex, const U& value) {
			kpp[kingIndex][pieceIndex] += value;
		}

		void addKPP(int index, const U& value) {
			kpp[0][index] += value;
		}

		// index1 >= index2
		U getKPP(int kingIndex, int index1, int index2) const {
			return kpp[kingIndex][getKPPIndex(index1, index2)];
		}

		U getKPP(int kingIndex, int pieceIndex) const {
			return kpp[kingIndex][pieceIndex];
		}

		U getKPP(int index) const {
			return kpp[0][index];
		}

		void setKKP(int kingIndex1, int kingIndex2, int pieceIndex, const U& value) {
			kkp[kingIndex1][kingIndex2][pieceIndex] = value;
		}

		void setKKP(int index, const U& value) {
			kkp[0][0][index] = value;
		}

		void setKKP(const Kings& kings, int pieceIndex,
				int squareIndex, bool black, const U& value) {
			if (black) {
				kkp[kings.getBlack()][kings.getWhite()]
					[pieceIndex+KKP_KNUM*squareIndex] = value;
			} else {
				kkp[kings.getBlackR()][kings.getWhiteR()]
					[pieceIndex+KKP_KNUM*squareIndex] = -value;
			}
		}

		void setKKP(const Kings& kings, const Shogi::Piece& piece,
				const Shogi::Square& sq, const U& value) {
			if (piece.isBlack()) {
				getKKP(kings, blackPiece[piece.getInteger()],
					sq.getShortIndex(), true) = value;
			} else {
				getKKP(kings, whitePiece[piece.getInteger()],
					80-sq.getShortIndex(), false) = -value;
			}
		}

		void addKKP(int kingIndex1, int kingIndex2, int pieceIndex, const U& value) {
			kkp[kingIndex1][kingIndex2][pieceIndex] += value;
		}

		void addKKP(int index, const U& value) {
			kkp[0][0][index] += value;
		}

		void addKKP(const Kings& kings, int pieceIndex,
				int squareIndex, bool black, const U& value) {
			if (black) {
				kkp[kings.getBlack()][kings.getWhite()]
					[pieceIndex+KKP_KNUM*squareIndex] += value;
			} else {
				kkp[kings.getBlackR()][kings.getWhiteR()]
					[pieceIndex+KKP_KNUM*squareIndex] -= value;
			}
		}

		void addKKP(const Kings& kings, const Shogi::Piece& piece,
				const Shogi::Square& sq, const U& value) {
			if (piece.isBlack()) {
				getKKP(kings, blackPiece[piece.getInteger()],
					sq.getShortIndex(), true) += value;
			} else {
				getKKP(kings, whitePiece[piece.getInteger()],
					80-sq.getShortIndex(), false) -= value;
			}
		}

		void addKKP_H(const Kings& kings, int pieceIndex, int count, bool black, const U& value) {
			if (black) {
				kkp[kings.getBlack()][kings.getWhite()][pieceIndex+count] += value;
			} else {
				-kkp[kings.getBlackR()][kings.getWhiteR()][pieceIndex+count] += value;
			}
		}

		U getKKP(int kingIndex1, int kingIndex2, int pieceIndex) const {
			return kkp[kingIndex1][kingIndex2][pieceIndex];
		}

		U getKKP(int index) const {
			return kkp[0][0][index];
		}

		U getKKP(const Kings& kings, int pieceIndex,
				int squareIndex, bool black) const {
			return black ? kkp[kings.getBlack()][kings.getWhite()]
				[pieceIndex+KKP_KNUM*squareIndex] :
				-kkp[kings.getBlackR()][kings.getWhiteR()]
				[pieceIndex+KKP_KNUM*squareIndex];
		}

		U getKKP(const Kings& kings, const Shogi::Piece& piece,
				const Shogi::Square& sq) const {
			if (piece.isBlack()) {
				return getKKP(kings, blackPiece[piece.getInteger()],
					sq.getShortIndex(), true);
			} else {
				return -getKKP(kings, whitePiece[piece.getInteger()],
					80-sq.getShortIndex(), false);
			}
		}

		U getKKP_H(const Kings& kings, int pieceIndex, int count, bool black) const {
			return black ? kkp[kings.getBlack()][kings.getWhite()][pieceIndex+count] :
				-kkp[kings.getBlackR()][kings.getWhiteR()][pieceIndex+count];
		}

		U sum() const {
			U value = 0;
			for (int i = 0; i < KPP_ALL; i++) {
				value += kpp[0][i];
			}
			for (int i = 0; i < KKP_ALL; i++) {
				value += kkp[0][0][i];
			}
			return value;
		}

		double ave() const {
			return (double)sum() / (KPP_ALL + KKP_ALL);
		}

		Type& operator+=(Type& param) {
			for (Shogi::Piece p = Shogi::Piece::PAWN; p <= Shogi::Piece::DRAGON; p.toNext()) {
				piece[p] += param.piece[p];
			}

			for (int i = 0; i < KPP_ALL; i++) {
				kpp[0][i] += param.kpp[0][i];
			}

			for (int i = 0; i < KKP_ALL; i++) {
				kkp[0][0][i] += param.kkp[0][0][i];
			}

			return (*this);
		}
	};

	typedef TempParam<ValueS, ValueS> Param;
	typedef TempParam<ValueD, ValueF> Gradient;
};

#endif // PARAM_H_
