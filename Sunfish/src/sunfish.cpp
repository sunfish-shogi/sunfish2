/*
 * sunfish.cpp
 *
 *  Created on: 2012/05/08
 *      Author: ryosuke
 */

#include "Search/pruningExpr.h"
#include "Cui/controller.h"
#include "Shogi/position.h"
#include "sunfish.h"
#include "Network/csaClient.h"
#include "Log/logger.h"
#include "Learn/learn.h"
#include "Evaluates/paramAnalyzer.h"
#include "Books/bookManager.h"
#include <iostream>
#include <fstream>
#include <boost/program_options.hpp>

#ifndef NDEBUG
#include "Test/shogiTest.h"
#include "Test/evalTest.h"
#include "Test/timeTest.h"
#endif

using boost::program_options::options_description;
using boost::program_options::variables_map;
using boost::program_options::value;
using boost::program_options::store;
using boost::program_options::parse_command_line;

#ifndef NDEBUG
bool test();
bool timeTest();
#endif //NDEBUG
bool learn();
bool analyze();
bool network();
bool book(const std::string& directory, int limit);
bool bookOne(const std::string& path, int limit);

static const int DEFAULT_BOOK_LIMIT = 60;

int main(int argc, char* argv[]) {
	// information
	std::cout << SUNFISH_NAME << ' ';
	std::cout << SUNFISH_VERSION << '\n';
	std::cout << SUNFISH_COPYRIGHT << '\n';

	// ログの出力先
	Log::error.addStream(std::cerr, "\x1b[31m", "\x1b[39m");
	Log::warning.addStream(std::cerr, "\x1b[33m", "\x1b[39m");
	Log::message.addStream(std::cerr);
	Log::send.addStream(std::cerr, "\x1b[34m", "\x1b[39m");
	Log::receive.addStream(std::cerr, "\x1b[35m", "\x1b[39m");
#ifndef NDEBUG
	Log::debug.addStream(std::cerr, "\x1b[36m", "\x1b[39m");
	Log::test.addStream(std::cerr, "\x1b[32m", "\x1b[39m");
#endif

	// hash
	unsigned hashSize = Shogi::PositionHash::getRequireSize();
	Util::Hash hash(hashSize);
	Shogi::PositionHash posHash(hash);
	Shogi::Position::setPositionHash(&posHash);

	// program options
	options_description opt("Option");
	opt.add_options()
			("help,h", "show help")
			("book", value<std::string>(), "import records into opening-book.")
			("book-one", value<std::string>(), "import one record into opening-book.")
			("book-limit", value<int>(), "limit of opening-book.(default:60)")
			("network,n", "CSA client moode")
			("auto-black,b", "search will be begun automatically on black turn.")
			("auto-white,w", "search will be begun automatically on white turn.")
			("depth,d", value<int>(), "search depth")
			("limit,l", value<int>(), "search limit(sec)")
			("parallel,p", value<int>(), "number of worker threads")
			("file,f", value<std::string>(), "CSA file name to read")
			("auto-quit,q", "automatical quit when a computer resign.")
#ifdef PRUN_EXPR
			("repeat,r", value<int>(), "(contains -b -w -q)")
#endif
#ifndef NDEBUG
			("test", "unit test")
			("test-time", "test for Util::Timer")
#endif //NDEBUG
#ifndef NLEARN
			("learn", "learning")
			("analyze", "analyzing for 'evdata'")
#endif //NLEARN
			;
	variables_map argmap;
	try {
		store(parse_command_line(argc, argv, opt), argmap);
	} catch (std::exception& e) {
		std::cerr << "ERROR :" << e.what() << std::endl;
		std::cerr << opt << std::endl;
		return 1;
	}

	if (argmap.count("help")) {
		// ** コマンドヘルプの表示
		std::cerr << opt << std::endl;
		return 0;
#ifndef NDEBUG
	} else if (argmap.count("test")) {
		// ** ユニットテスト
		return test() ? 0 : 1;
	} else if (argmap.count("test-time")) {
		// ** 時間計測機能のテスト
		return timeTest() ? 0 : 1;
#endif //NDEBUG
#ifndef NLEARN
	} else if (argmap.count("learn")) {
		// ** 機械学習
		return learn() ? 0 : 1;
	} else if (argmap.count("analyze")) {
		// ** パラメータの解析
		return analyze() ? 0 : 1;
#endif //NLEARN
	} else if (argmap.count("book")) {
		// ** ディレクトリから全ての棋譜を定跡にインポート
		std::string directory = argmap["book"].as<std::string>();
		int limit = DEFAULT_BOOK_LIMIT;
		if (argmap.count("book-limit")) {
			limit = argmap["book-limit"].as<int>();
		}
		return book(directory, limit) ? 0 : 1;
	} else if (argmap.count("book-one")) {
		// ** 1つの棋譜を定跡にインポート
		std::string path = argmap["book-one"].as<std::string>();
		int limit = DEFAULT_BOOK_LIMIT;
		if (argmap.count("book-limit")) {
			limit = argmap["book-limit"].as<int>();
		}
		return bookOne(path, limit) ? 0 : 1;
	} else if (argmap.count("network")) {
		// ** CSA Client の起動
		return network() ? 0 : 1;
	}

	// ** CLI の起動
	Cui::Controller controller;
	if (argmap.count("auto-black")) {
		controller.setAutoBlack(true);
	}
	if (argmap.count("auto-white")) {
		controller.setAutoWhite(true);
	}
	if (argmap.count("depth")) {
		controller.setDepth(argmap["depth"].as<int>());
	}
	if (argmap.count("limit")) {
		controller.setLimit(argmap["limit"].as<int>());
	}
	if (argmap.count("parallel")) {
		controller.setWorker(argmap["parallel"].as<int>());
	}
	if (argmap.count("file")) {
		controller.setFilename(argmap["file"].as<std::string>().c_str());
	}
	if (argmap.count("auto-quit")) {
		controller.setAutoQuit(true);
	}
#ifdef PRUN_EXPR
std::ofstream exprOut("expr.csv", std::ios::out | std::ios::app);
Log::expr.addStream(exprOut);
int repeat = 1;
if (argmap.count("repeat")) {
	controller.setAutoBlack(true);
	controller.setAutoWhite(true);
	controller.setAutoQuit(true);
	repeat = argmap["repeat"].as<int>();
}
for (int i = 0; i < repeat; i++) {
#endif

	controller.play();

#ifdef PRUN_EXPR
	Search::PruningExpr::print();
}
exprOut.close();
#endif

	return 0;
}

#ifndef NDEBUG
// ユニットテスト
bool test() {
	using namespace Tests;
	// ログの出力先に test.log を追加
	std::ofstream fout("test.log", std::ios::out | std::ios::app);
	if (fout) {
		Log::error.addStream(fout, true);
		Log::warning.addStream(fout, true);
		Log::message.addStream(fout, true);
		Log::send.addStream(fout, true);
		Log::receive.addStream(fout, true);
#ifndef NDEBUG
		Log::debug.addStream(fout, true);
		Log::test.addStream(fout, true);
#endif
	}
	// テストの実行
	bool result = true;
	result &= ShogiTest().test();
	result &= EvalTest().test();
	fout.close();
	return result;
}

bool timeTest() {
	using namespace Tests;
	return TimeTest().test();
}
#endif //NDEBUG

#ifndef NLEARN
// 機械学習
bool learn() {
	using namespace Learns;
	// ログの出力先に learn.log を追加
	std::ofstream fout("learn.log", std::ios::out | std::ios::app);
	if (fout) {
		Log::error.addStream(fout, true);
		Log::warning.addStream(fout, true);
		Log::message.addStream(fout, true);
	}
	// 機械学習の実行
	Learn learn;
	learn.execute();
	fout.close();
	return true;
}
#endif

// パラメータの解析
bool analyze() {
	using namespace Evaluates;
	Param* pparam = new Param("evdata");
	ParamAnalyzer analyzer(*pparam);
	std::cout << analyzer.analyze();
	delete pparam;
	return true;
}

// CSA Client の起動
bool network() {
	// ログの出力先に network.log を追加
	std::ofstream fout("network.log", std::ios::out | std::ios::app);
	if (fout) {
		Log::error.addStream(fout, true);
		Log::warning.addStream(fout, true);
		Log::message.addStream(fout, true);
		Log::send.addStream(fout, true);
		Log::receive.addStream(fout, true);
#ifndef NDEBUG
		Log::debug.addStream(fout, true);
#endif
	}
	// client 起動
	Network::CsaClient csaClient;
	csaClient.execute();
	fout.close(); // close a log file.
	return true;
}

bool book(const std::string& directory, int limit) {
	Books::BookManager manager;
	manager.importDirectory(directory.c_str(), limit);
	manager.write();
	return true;
}

bool bookOne(const std::string& path, int limit) {
	Books::BookManager manager;
	manager.importFile(path.c_str(), limit);
	manager.write();
	return true;
}
