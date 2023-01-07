/*
 * param.cpp
 *
 *  Created on: 2012/07/07
 *      Author: ryosuke
 */

#include <fstream>
#include <cassert>
#include "param.h"
#include "../Log/logger.h"

namespace Evaluates {
	using namespace Shogi;

	const int blackPiece[] = {
		 0,
//		歩  香  桂  銀  金  角  飛  王
		 0,  1,  2,  3,  4,  5,  6,  0,
		 4,  4,  4,  4,  0,  7,  8,  0,
//		歩  香  桂  銀  金  角  飛  王
		 9, 10, 11, 12, 13, 14, 15,  0,
		13, 13, 13, 13,  0, 16, 17,
	};

	const int whitePiece[] = {
		 0,
//		歩  香  桂  銀  金  角  飛  王
		 9, 10, 11, 12, 13, 14, 15,  0,
		13, 13, 13, 13,  0, 16, 17,  0,
//		歩  香  桂  銀  金  角  飛  王
		 0,  1,  2,  3,  4,  5,  6,  0,
		 4,  4,  4,  4,  0,  7,  8,
	};

	// 座標の左右反転
	const int sym[] = {
	 	 8,  7,  6,  5,  4,  3,  2,  1,  0,
		17, 16, 15, 14, 13, 12, 11, 10,  9,
		26, 25, 24, 23, 22, 21, 20, 19, 18,
		35, 34, 33, 32, 31, 30, 29, 28, 27,
		44, 43, 42, 41, 40, 39, 38, 37, 36,
		53, 52, 51, 50, 49, 48, 47, 46, 45,
		62, 61, 60, 59, 58, 57, 56, 55, 54,
		71, 70, 69, 68, 67, 66, 65, 64, 63,
		80, 79, 78, 77, 76, 75, 74, 73, 72,
	};

	template <class T, class U>
	void TempParam<T, U>::init() {
		memset((char*)piece, 0, sizeof(piece));
		memset((char*)kpp, 0, sizeof(kpp));
		memset((char*)kkp, 0, sizeof(kkp));
		cumulate();
	}
	template void TempParam<ValueS, ValueS>::init();
	template void TempParam<ValueD, ValueF>::init();

	template <class T, class U>
	bool TempParam<T, U>::read(const char* filename) {
		Log::message.fileOpenMessage(filename);
		std::ifstream fin(filename, std::ios::in | std::ios::binary);
		if (!fin) {
			Log::error.fileOpenError(filename);
			return false;
		}
		fin.read((char*)&piece[1], sizeof(piece[0])*Piece::DRAGON);
		fin.read((char*)kpp, sizeof(kpp));
		fin.read((char*)kkp, sizeof(kkp));
		if (fin.fail()) {
			Log::error.fileIoError(filename);
			return false;
		}
		fin.close();
		cumulate();
		return true;
	}
	template bool TempParam<ValueS, ValueS>::read(const char* filename);
	template bool TempParam<ValueD, ValueF>::read(const char* filename);

	template <class T, class U>
	bool TempParam<T, U>::write(const char* filename) {
		Log::message.fileOpenMessage(filename);
		std::ofstream fout(filename, std::ios::out | std::ios::binary);
		if (!fout) {
			Log::error.fileOpenError(filename);
			return false;
		}
		fout.write((char*)&piece[1], sizeof(piece[0])*Piece::DRAGON);
		fout.write((char*)kpp, sizeof(kpp));
		fout.write((char*)kkp, sizeof(kkp));
		if (fout.fail()) {
			Log::error.fileIoError(filename);
			return false;
		}
		fout.close();
		return true;
	}
	template bool TempParam<ValueS, ValueS>::write(const char* filename);
	template bool TempParam<ValueD, ValueF>::write(const char* filename);

	/*
	 * パラメータの値を足し込む。
	 * パラメータの内容が変更された時に呼び出す。
	 */
	template <class T, class U>
	void TempParam<T, U>::cumulate() {
		piece[Piece::EMPTY] = pieceEx[Piece::EMPTY]
				= piecePr[Piece::EMPTY] = Value(0);
		for (Piece p = Piece::PAWN; p <= Piece::ROOK; p.toNext()) {
			Shogi::Piece pp = p.getPromoted();
			pieceEx[p] = piece[p] * 2;
			pieceEx[pp] = piece[p] + piece[pp];
			piecePr[p] = piecePr[pp] = piece[pp] - piece[p];
		}
	}
	template void TempParam<ValueS, ValueS>::cumulate();
	template void TempParam<ValueD, ValueF>::cumulate();

	/*
	 * cumulateの逆変換を行う。
	 * 3駒関係を用いる場合は記述すべき処理はない。
	 */
	template <class T, class U>
	void TempParam<T, U>::decumulate() {
	}
	template void TempParam<ValueS, ValueS>::decumulate();
	template void TempParam<ValueD, ValueF>::decumulate();
}
