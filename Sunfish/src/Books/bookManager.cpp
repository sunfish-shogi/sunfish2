/*
 * bookManager.cpp
 *
 *  Created on: 2013/03/14
 *      Author: ryosuke
 */

#include "bookManager.h"
#include "../Log/logger.h"
#include "../Csa/csaReader.h"
#include "../Util/fileList.h"

namespace Books {
	using namespace Records;
	using namespace Csa;
	using namespace Shogi;
	using namespace Util;

	bool BookManager::importFile(
			const char* path, int threshold) {
		// 棋譜の読み込み
		Record record;
		if (!CsaReader::read(path, record)) {
			return false;
		}
		// 初手から順に棋譜を見ていく。
		record.begin();
		while (threshold == IMPORT_UNLIMITED ||
				record.getCurrent() < threshold) {
			Move move;
			if (!record.getNextMove(move)) {
				break;
			}
			book.addMove(record.getPosition().getHash(), move);
			record.next();
		}
		return true;
	}

	bool BookManager::importDirectory(
			const char* directory, int threshold) {
		// 棋譜の列挙
		FileList fileList;
		fileList.enumerate(directory, "csa");
		int count = 0;
		while (!fileList.isEnd()) {
			const std::string& path = fileList.pop();
			count++;
			Log::message << count << '\t' << path << '\n';
			if (!importFile(path.c_str(), threshold)) {
				return false;
			}
		}
		return true;
	}
}
