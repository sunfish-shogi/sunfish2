/*
 * csaClientConfig.cpp
 *
 *  Created on: 2012/09/08
 *      Author: ryosuke
 */

#include "csaClientConfig.h"
#include "../Log/logger.h"
#include <fstream>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

namespace Network {
	using namespace Configures;

	CsaClientConfig::CsaClientConfig(const char* filename) {
		int i = 0;
		items[i++] = ConfigItem("host", STRING, &host, "localhost");
		items[i++] = ConfigItem("port", INTEGER, &port, "4081");
		items[i++] = ConfigItem("user", STRING, &user, "test");
		items[i++] = ConfigItem("pass", STRING, &pass, "");
		items[i++] = ConfigItem("depth", INTEGER, &depth, "32");
		items[i++] = ConfigItem("limit", INTEGER, &limit, "10");
		items[i++] = ConfigItem("repeat", INTEGER, &repeat, "1");
		items[i++] = ConfigItem("worker", INTEGER, &worker, "1");
		items[i++] = ConfigItem("enemy", BOOL, &enemy, "1");
		items[i++] = ConfigItem("keepalive", INTEGER, &keepalive, "1");
		items[i++] = ConfigItem("keepidle", INTEGER, &keepidle, "120");
		items[i++] = ConfigItem("keepintvl", INTEGER, &keepintvl, "60");
		items[i++] = ConfigItem("keepcnt", INTEGER, &keepcnt, "10");
		items[i++] = ConfigItem("floodgate", BOOL, &floodgate, "0");
		items[i++] = ConfigItem("kifu", STRING, &kifu, "Kifu");

		if (filename != NULL) {
			read(filename);
		}
	}
}
