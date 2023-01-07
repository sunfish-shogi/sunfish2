/*
 * controller.cpp
 *
 *  Created on: 2012/07/01
 *      Author: ryosuke
 */

#include "controller.h"
#include "../Util/tableString.h"
#include "../Search/searcher.h"
#include "../Search/attackers.h"
#include "../Csa/csaReader.h"
#include "../Csa/csaWriter.h"

namespace Cui {
	using namespace Shogi;
	using namespace Search;
	using namespace Evaluates;
	using namespace Records;
	using namespace Csa;
	using namespace Books;

	const Controller::CommandSet Controller::commandSet[CMD_NUM] = {
		{ "q", "quit", QUIT, "quit." },
		{ "h", "help", HELP, "show this help." },
		{ "p", "prev", PREV, "go to a previous position." },
		{ "n", "next", NEXT, "go to a next position." },
		{ "t", "top", TOP, "go to a top of this record." },
		{ "e", "end", END, "go to a end of this record." },
		{ "s", "search", SEARCH, "search from current position." },
		{ "m", "moves", MOVES, "show legal moves." },
		{ "c", "cap", CAPTURES, "show capturing moves." },
		{ "nc", "ncap", NOCAPTURES, "show not captureing moves." },
		{ "k", "check", CHECK, "show checks." },
		{ "d", "mated", MATED, "is mated?" },
		{ "1", "mate1", MATE1, "search check-mate on 1 ply." },
#ifndef NDEBUG
		{ NULL, "see", SEE, "static exchange evaluation test.(DEBUG)" },
		{ NULL, "eval", EVALUATE, "show evaluation.(DEBUG)" },
#endif // ifndef NDEBUG
		{ NULL, "book", BOOK, "show opening-book moves" },
	};

	Controller::Command Controller::inputCommand(const char* str) {
		if (str[0] == '\0') {
			return EMPTY;
		}
		for (int i = 0; i < CMD_NUM; i++) {
			const char* s = commandSet[i].shortStr;
			const char* l = commandSet[i].longStr;
			if ((s != NULL && 0 == strcmp(str, s)) ||
					(l != NULL && 0 == strcmp(str, l))) {
				return commandSet[i].command;
			}
		}
		return UNKNOWN;
	}

	bool Controller::inputMove(const char* str, const Position& pos, Move& move) const {
		if (strlen(str) < 4) {
			return false;
		}
		Square to = Square::parse(&str[2]);
		if (!to.valid()) {
			return false;
		}
		Square from = Square::parse(&str[0]);
		if (from.valid()) {
			Piece piece = pos.getBoard(Square(from));
			move.setPromotion(str[4] == 'n');
			move.setFrom(from);
			move.setTo(to);
			move.setHand(false);
			move.setPiece(piece);
			return true;
		} else {
			Piece piece = Piece::parseCsa(&str[0]);
			if (piece >= Piece::PAWN && piece <= Piece::ROOK) {
				move.setPromotion(false);
				move.setFrom(Square::NON);
				move.setTo(to);
				move.setHand(true);
				move.setPiece(pos.isBlackTurn() ? piece : piece.getTurnedWhite());
				return true;
			}
		}
		return false;
	}

	void Controller::showHelp() {
		for (int i = 0; i < CMD_NUM; i++) {
			const char* l = commandSet[i].longStr;
			const char* s = commandSet[i].shortStr;
			const char* d = commandSet[i].description;
			if (l != NULL) { std::cout << l; }
			std::cout << '\t';
			if (s != NULL) { std::cout << s; }
			std::cout << '\t';
			if (d != NULL) { std::cout << d; }
			std::cout << '\n';
		}
	}

	void Controller::showLegalMoves(const Position& pos) {
		MoveGenerator gen(pos);
		gen.generate();
		showMoves(gen);
	}

	void Controller::showCaptures(const Position& pos) {
		MoveGenerator gen(pos);
		gen.generateCapture();
		showMoves(gen);
	}

	void Controller::showNoCaptures(const Position& pos) {
		MoveGenerator gen(pos);
		gen.generateNocapture();
		showMoves(gen);
	}

	void Controller::showCheck(const Position& pos) {
		MoveGenerator gen(pos);
		gen.generateCheck();
		showMoves(gen);
	}

	void Controller::showMoves(MoveGenerator& gen) {
		const Move* pmove;
		while ((pmove = gen.next()) != NULL) {
			std::cout << pmove->toString() << ' ';
		}
		std::cout << '\n';
	}

	void Controller::seeTest(const Position& pos, const Param& param) {
		MoveGenerator gen(pos);
		gen.generateCapture();
		const Move* pmove;
		while ((pmove = gen.next()) != NULL) {
			Attackers attackers(param, pos, *pmove);
			std::cout << pmove->toString() << ':' << (int)attackers.see() << '\n';
		}
	}

	void Controller::evaluate(const Position& pos, const Param& param) {
		Evaluate eval(param, pos);
		std::cout << eval.getValue() << '\n';
	}

	void Controller::showBook(const Shogi::Position& pos, const Books::BookManager& book) {
		const BookMoves* moves = book.getMoves(pos.getHash());
		if (moves == NULL) {
			std::cout << "there is no moves.\n";
		} else {
			for (unsigned index = 0; index < moves->getSize(); index++) {
				std::cout << moves->getMove(index)->toString()
						<< '[' << moves->getCount(index) << "] ";
			}
			std::cout << '\n';
		}
	}

	void Controller::printPosition(const Record& record) const {
		std::cout << record.toString();
#ifndef NDEBUG
		Util::TableString table("", "=", "");
		table.row() << "POSITION_HASH" << Util::Int::toString64(record.getPosition().getHash());
		table.row() << "BOARD_HASH" << Util::Int::toString64(record.getPosition().getBoardHash());
		table.row() << "HAND_HASH" << Util::Int::toString64(record.getPosition().getHandHash());
		table.row() << "TURN_HASH" << Util::Int::toString64(record.getPosition().getTurnHash());
		Log::debug << table.get();
#endif
	}

	void Controller::printStatus(const Record& record) const {
		if (record.getPosition().isMate()) {
			std::cout << "mate!!\n";
		}
		if (record.isRepetition()) {
			std::cout << "repetition!!\n";
		}
	}

	bool Controller::play() {
		char line[1024];
		Command prevCommand = EMPTY;
		Record record;
		Searcher searcher(*pparam, config.worker);
		SearchConfig searchConfig = SearchConfig::getDefault();
		SearchResult result;
		BookManager book;

		// 棋譜読み込み
		if (config.filename != NULL) {
			CsaReader::read(config.filename, record);
		}

		// 探索設定
		searchConfig.depth = config.depth;
		searchConfig.pvHandler = this;
		searchConfig.limitEnable = (config.limit > 0);
		searchConfig.limitSeconds = config.limit;
		searcher.setSearchConfig(searchConfig);

		printPosition(record);
		printStatus(record);

		while (true) {
#ifdef PRUN_EXPR
			if (record.getCurrent() >= 200) {
				break;
			}
#endif
			bool print = false;
			Move move;

#ifndef NDEBUG
			CsaWriter::write("debug.csa", record);
#endif

			// コンピュータによる着手
			if ((record.getPosition().isBlackTurn() && config.autoBlack) ||
					(record.getPosition().isWhiteTurn() && config.autoWhite)) {
				// 定跡
				const Move* pmove = book.getMove(record.getPosition().getHash());
				if (pmove != NULL) {
					std::cout << "opening-book.\n";
				} else {
					// 探索
					searcher.init(record);
					searcher.idSearch(result);
					std::cout << result.toString();
					if (!result.resign) {
						pmove = &result.move;
					}
				}
				// 着手
				if (pmove == NULL || !record.move(*pmove)) {
					// 自動終了
					if (config.autoQuit) {
						std::cout << "quit.\n";
						break;
					}
					// 着手をマニュアルモードに切り替え
					if (config.autoBlack) {
						std::cout << "black :auto => manual\n";
						config.autoBlack = false;
					}
					if (config.autoWhite) {
						std::cout << "white :auto => manual\n";
						config.autoWhite = false;
					}
				}
				std::cout << record.toString();
				continue;
			}

			// ユーザからのコマンド入力
			std::cin.getline(line, sizeof(line));
			if (std::cin.eof()) { break; }
			Command command = line[0] != '\0' ? inputCommand(line) : prevCommand;
			if (command == QUIT) { break; }
			prevCommand = EMPTY;
			switch(command) {
			case EMPTY:
				break;
			case HELP:
				showHelp();
				break;
			case PREV: // 1手進む。
				if (record.prev()) {
					print = true;
				} else {
					std::cout << "There is no previous move.\n";
				}
				if (config.autoBlack || config.autoWhite) {
					record.prev();
				}
				prevCommand = PREV;
				break;
			case NEXT: // 1手戻る。
				if (record.next()) {
					print = true;
				} else {
					std::cout << "There is no next move.\n";
				}
				if (config.autoBlack || config.autoWhite) {
					record.next();
				}
				prevCommand = NEXT;
				break;
			case TOP:
				while (record.prev()) {
					print = true;
				}
				if (!print) {
					std::cout << "There is no previous move.\n";
				}
				break;
			case END:
				while (record.next()) {
					print = true;
				}
				if (!print) {
					std::cout << "There is no next move.\n";
				}
				break;
			case MOVES: // 指し手列挙
				showLegalMoves(record.getPosition());
				break;
			case CAPTURES: // 駒を取る手列挙
				showCaptures(record.getPosition());
				break;
			case NOCAPTURES: // 駒を取らない手列挙
				showNoCaptures(record.getPosition());
				break;
			case CHECK: // 王手
				showCheck(record.getPosition());
				break;
			case MATE1: // 1手詰み
				if (searcher.isMate1Ply(record.getPosition())) {
					std::cout << "mate.\n";
				} else {
					std::cout << "no mate.\n";
				}
				break;
			case MATED:
				if (record.getPosition().isMate()) {
					std::cout << "mate.\n";
				} else {
					std::cout << "no mate.\n";
				}
				break;
			case SEARCH: // 探索
				searcher.init(record);
				searcher.idSearch(result);
				std::cout << result.toString();
				if (result.resign) {
					std::cout << "lose.\n";
				}
				break;
#ifndef NDEBUG
			case SEE:
				seeTest(record.getPosition(), *pparam);
				break;
			case EVALUATE:
				evaluate(record.getPosition(), *pparam);
				break;
#endif // ifndef NDEBUG
			case BOOK:
				showBook(record.getPosition(), book);
				break;
			default: // 指し手入力
				if (CsaReader::parseLineMove(line, record.getPosition(), move) ||
						inputMove(line, record.getPosition(), move)) {
					// 合法手チェック
					if (record.move(move)) {
						std::cout << move.toString() << '\n';
						print = true;
					} else {
						std::cout << "illegal move!!\n";
					}
				} else {
					std::cout << "unknown command!!\n";
				}
			}

			// 盤面の表示
			if (print) {
				printPosition(record);
				printStatus(record);
			}
		}

		return true;
	}
}
