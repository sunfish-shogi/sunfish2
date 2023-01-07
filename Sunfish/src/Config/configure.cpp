/*
 * configure.cpp
 *
 *  Created on: 2013/02/03
 *      Author: ryosuke
 */

#include "configure.h"
#include "../Log/logger.h"
#include "../Util/tableString.h"
#include <fstream>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

namespace Configures {
	bool Configure::read(const char* filename) {
		// open a configure file.
		Log::message.fileOpenMessage(filename);
		std::ifstream fin(filename);
		if (!fin) {
			Log::error.fileOpenError(filename);
			return false;
		}
		// set default values
		init();
		// input
		for (int l = 0; ; l++) {
			char line[LINE_BUFFER_SIZE];
			fin.getline(line, sizeof(line));
			if (fin.eof()) { break; }
			if (fin.fail()) {
				Log::error.fileIoError(filename);
				return false;
			}
			if (!readLine(line)) {
				Log::error.fileFormatError(filename, l, line);
			}
		}
		fin.close(); // close a configure file.
		return true;
	}

	void Configure::init() {
		// 初期値を代入
		ConfigItem* items = itemList();
		int size = itemSize();
		for (int i = 0; i < size; i++){
			// 設定項目のデータ型毎に変換
			if (!convert(items[i], items[i].defaultValue)) {
				Log::error.unknownError();
			}
		}
	}

	bool Configure::readLine(const char* line) {
		if (line[0] == '\0' || line[0] == '#') {
			return true;
		}
		// '=' で左辺値と右辺値に分解
		std::vector<std::string> tokens;
		boost::algorithm::split(tokens, line, boost::is_any_of("="));
		if (tokens.size() != 2) {
			return false;
		}
		// 左辺値に一致する項目を探す。
		ConfigItem* items = itemList();
		int size = itemSize();
		for (int i = 0; i < size; i++){
			if (tokens[0] == items[i].name) {
				// 設定項目のデータ型毎に変換
				if (convert(items[i], tokens[1])) {
					return true;
				} else {
					Log::error.unknownError();
					return false;
				}
			}
		}
		return false;
	}

	bool Configure::convert(ConfigItem& item, const std::string& str) {
		// 設定項目のデータ型毎に変換
		if (item.type == STRING) {
			*(std::string*)item.data = str;
		} else if (item.type == INTEGER) {
			*(int*)item.data = boost::lexical_cast<int>(str);
		} else if (item.type == BOOL) {
			*(bool*)item.data = boost::lexical_cast<bool>(str);
		} else {
			return false;
		}
		return true;
	}

	std::string Configure::toString() {
		Util::TableString table(":");
		ConfigItem* items = itemList();
		int size = itemSize();
		for (int i = 0; i < size; i++){
			table.row() << items[i].name;
			if (items[i].type == STRING) {
				table << *(std::string*)items[i].data;
			} else if (items[i].type == INTEGER) {
				table << *(int*)items[i].data;
			} else if (items[i].type == BOOL) {
				table << *(bool*)items[i].data;
			} else {
				table << "error";
			}
		}
		return table.get();
	}
}
