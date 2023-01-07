/*
 * csaClientConfig.h
 *
 *  Created on: 2012/09/08
 *      Author: ryosuke
 */

#ifndef CSACLIENTCONFIG_H_
#define CSACLIENTCONFIG_H_

#include "../Config/configure.h"

namespace Network {
	class CsaClientConfig : public Configures::Configure {
	private:
		Configures::ConfigItem items[15];

		std::string host;
		int port;
		std::string user;
		std::string pass;

		int depth;
		int limit;
		int repeat;
		int worker;
		bool enemy;

		int keepalive;
		int keepidle;
		int keepintvl;
		int keepcnt;

		bool floodgate;

		std::string kifu;

	protected:
		Configures::ConfigItem* itemList() {
			return items;
		}

		int itemSize() {
			return sizeof(items)/sizeof(items[0]);
		}

	public:
		CsaClientConfig(const char* filename = NULL);
		
		const std::string& getHost() const {
			return host;
		}

		int getPort() const {
			return port;
		}

		const std::string& getUser() const {
			return user;
		}

		const std::string& getPass() const {
			return pass;
		}

		int getDepth() const {
			return depth;
		}

		int getLimit() const {
			return limit;
		}

		int getRepeat() const {
			return repeat;
		}

		int getWorker() const {
			return worker;
		}

		bool getEnemy() const {
			return enemy;
		}

		int getKeepalive() const {
			return keepalive;
		}

		int getKeepidle() const {
			return keepidle;
		}

		int getKeepintvl() const {
			return keepintvl;
		}

		int getKeepcnt() const {
			return keepcnt;
		}

		bool getFloodgate() const {
			return floodgate;
		}

		std::string getKifu() const {
			// TODO: 共通化
			if (kifu.length() > 0 && kifu[kifu.length()-1] == '/') {
				return kifu;
			} else {
				return kifu + '/';
			}
		}
	};
}

#endif // CSACLIENTCONFIG_H_
