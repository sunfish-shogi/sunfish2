/*
 * csaReader.cpp
 * 
 *  Created on: 2012/06/09
 *      Author: Kubo Ryosuke
 */

#include "csaReader.h"
#include "../Shogi/square.h"
#include "../Tools/debug.h"
#include "../Log/logger.h"
#include <cstring>
#include <fstream>

namespace Csa {
	using namespace Shogi;

	bool CsaReader::parseLineBoard(const char* line, unsigned rank, Position& pos) {
		if (strlen(line) < 3 * Square::FILE_NUM) {
			return false;
		}
		for (int file = Square::FILE_NUM; file >= 1; file-- ) {
			Piece piece = Piece::parseCsa(line);
			pos.setBoard(Square(file, rank), piece);
			line += 3;
		}
		return true;
	}

	bool CsaReader::parseLineHand(const char* line, Position& pos, bool black) {
		while (strlen(line) >= 4) {
			unsigned file = line[0] - '0';
			unsigned rank = line[1] - '0';
			Piece piece = Piece::parseCsa(line+2);
			if (piece != Piece::EMPTY) {
				if (Square::isInside(file, rank)) {
					pos.setBoard(Square(file, rank), piece);
				} else if (file == 0 && rank == 0) {
					if (black) { pos.incBlackHand(piece); }
					else       { pos.incWhiteHand(piece); }
				} else {
					return false;
				}
			} else {
				return false;
			}
			line += 4;
		}
		return true;
	}

	bool CsaReader::parseLineMove(const char* line, const Position& pos, Move& move) {
		if (strlen(line) < 7) {
			return false;
		}
		bool black;
		if (line[0] == CHAR_BLK) {
			black = true;
		} else if (line[0] == CHAR_WHT) {
			black = false;
		} else {
			return false;
		}
		Square from = Square::parse(&line[1]);
		Square to = Square::parse(&line[3]);
		if (!from.valid() || !to.valid()) {
			return false;
		}
		bool hand = (from == Square::NON);
		Piece piece = Piece::parseCsa(&line[5]);
		if (piece == Piece::EMPTY) {
			return false;
		}
		black ? piece.turnBlack() : piece.turnWhite();
		if (hand) {
			if (!(pos.getHand(piece) > 0)) {
				return false;
			}
			move.setPromotion(false);
		} else {
			Piece pieceB = pos.getBoard(Square(from));
			if (piece == pieceB) {
				move.setPromotion(false);
			} else if (piece == pieceB.getPromoted()) {
				piece = pieceB;
				move.setPromotion(true);
			} else {
				return false;
			}
		}
		move.setFrom(from);
		move.setTo(to);
		move.setHand(hand);
		move.setPiece(piece);
		return true;
	}

	bool CsaReader::parseLineMove(const char* line, Records::Record& record) {
		Move move;
		return parseLineMove(line, record.getPosition(), move) && record.move(move);
	}

	CsaReader::LineStat CsaReader::parseLine(const char* line, Records::Record& record) {
		switch (line[0]) {
		case CHAR_BLK: // '+'
		case CHAR_WHT: // '-'
			return parseLineMove(line, record) ? LINE_MOVE : LINE_ERROR;
		case CHAR_TIM: // 'T'
			return LINE_TIME;
		case CHAR_STA: // '%'
			return LINE_STAT;
		case CHAR_COM: // '\''
		case CHAR_NON: // '\0'
			return LINE_EMPTY;
		default:
			return LINE_ERROR;
		}
	}

	CsaReader::LineStat CsaReader::parseLine(const char* line, Position& pos) {
		switch (line[0]) {
		case CHAR_POS: // 'P'
			if (line[1] >= '1' && line[1] <= '9') { // P1, P2, ..., P9
				return parseLineBoard(line+2, line[1]-'0', pos) ? LINE_BOARD : LINE_ERROR;
			} else if (line[1] == CHAR_BLK) { // P+
				return parseLineHand(line+2, pos, true) ? LINE_HAND : LINE_ERROR;
			} else if (line[1] == CHAR_WHT) { // P-
				return parseLineHand(line+2, pos, false) ? LINE_HAND : LINE_ERROR;
			}
			return LINE_ERROR;
		case CHAR_BLK: // '+'
			pos.setBlackTurn();
			return LINE_TURN;
		case CHAR_WHT: // '-'
			pos.setWhiteTurn();
			return LINE_TURN;
		case CHAR_VAR: // 'V'
			return LINE_VAR;
		case CHAR_PLR: // 'N'
		case CHAR_INF: // '$'
			return LINE_INFO;
		case CHAR_COM: // '\''
		case CHAR_NON: // '\0'
			return LINE_EMPTY;
		default:
			return LINE_ERROR;
		}
	}

	bool CsaReader::read(std::istream& in, Position& pos) {
		char line[LINE_BUFFER_SIZE];
		pos.initNoUpdate();
		while (true) {
			in.getline(line, sizeof(line));
			if (in.eof()) { break; }
			if (in.fail()) {
				Log::error.ioError();
				return false;
			}
			LineStat stat = parseLine(line, pos);
			if (stat == LINE_TURN) {
				break;
			} else if (stat == LINE_ERROR) {
				Log::error.formatError(line);
				return false;
			}
		}
		pos.update();
		return true;
	}

	bool CsaReader::read(const char* filename, Position& pos) {
		std::ifstream fin(filename);
		if (!fin) {
			Log::error.fileOpenError(filename);
			return false;
		}
		if (!read(fin, pos)) {
			fin.close();
			return false;
		}
		fin.close();
		return true;
	}

	bool CsaReader::read(const char* filename, Records::Record& record) {
		Position pos;
		std::ifstream fin(filename);
		if (!fin) {
			Log::error.fileOpenError(filename);
			return false;
		}

		// 局面の読み込み
		if (!read(fin, pos)) {
			fin.close();
			return false;
		}
		record.init(pos);

		// 指し手の読み込み
		while (true) {
			char line[LINE_BUFFER_SIZE];
			fin.getline(line, sizeof(line));
			if (fin.eof()) { break; }
			if (fin.fail()) {
				Log::error.fileIoError(filename);
				return false;
			}
			LineStat stat = parseLine(line, record);
			if (stat == LINE_ERROR) {
				Log::error.fileFormatError(filename, line);
				fin.close();
				return false;
			}
		}

		fin.close();
		return true;
	}
}
