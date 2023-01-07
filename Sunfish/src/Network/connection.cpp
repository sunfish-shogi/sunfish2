/*
 * connection.cpp
 *
 *  Created on: 2012/08/25
 *      Author: ryosuke
 */

#include "connection.h"
#define BOOST_THREAD_USE_LIB
#include <boost/thread/mutex.hpp>

#if WIN32
#include <windows.h>
#include <shlwapi.h>
#else
#include <unistd.h>
#endif

namespace Network {
	bool Connection::connect() {
		struct hostent* he;
		struct sockaddr_in sin;
#ifdef WIN32
		WORD    wVersionRequested = MAKEWORD(2,2);
		WSADATA WSAData;
		WSAStartup( wVersionRequested, &WSAData );
#endif

		if (NULL == (he = gethostbyname(host.c_str()))) {
			return false;
		}
		if (-1 == (sock = socket(AF_INET, SOCK_STREAM, 0))) {
			return false;
		}
#ifdef POSIX
		// keep-alive
		if (0 != setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE,
				(void*)&keepalive, sizeof(keepalive))) {
		}
		if (0 != setsockopt(sock, IPPROTO_TCP, TCP_KEEPIDLE,
				(void*)&keepidle, sizeof(keepidle))) {
		}
		if (0 != setsockopt(sock, IPPROTO_TCP, TCP_KEEPINTVL,
				(void*)&keepintvl, sizeof(keepintvl))) {
		}
		if (0 != setsockopt(sock, IPPROTO_TCP, TCP_KEEPCNT,
				(void*)&keepcnt, sizeof(keepcnt))) {
		}
#endif
		memcpy(&sin.sin_addr, he->h_addr, sizeof(struct in_addr));
		sin.sin_family = AF_INET;
		sin.sin_port = htons(port);
		if (-1 == ::connect(sock, (struct sockaddr*)(&sin), sizeof(sin))) {
			disconnect();
			return false;
		}

		return true;
	}

	void Connection::disconnect() {
#ifdef WIN32
		closesocket( sock );
		WSACleanup();
#else
		close( sock );
#endif
	}

	bool Connection::receive() {
		char buf[1024];

		if (!received.empty()) {
			return true;
		}

		memset( buf, 0, sizeof(buf) );
		if (recv(sock, buf, sizeof(buf) - 1, 0) > 0) {
			char* p = buf;
			while (p != NULL && p[0] != '\0') {
				char* p2 = strchr(p, '\n');
				if (p2 != NULL) {
					p2[0] = '\0';
					p2++;
				}
				received.push(std::string(p));
				p = p2;
			}
			return true;
		}
		return false;
	}

	bool Connection::send(const std::string& str) {
		if (-1 != ::send(sock, str.c_str(), str.length(), 0)) {
			return true;
		}
		return false;
	}

	bool Connection::sendln(const std::string& str) {
		std::string strln = str + '\n';
		if (-1 != ::send(sock, strln.c_str(), strln.length(), 0)) {
			return true;
		}
		return false;
	}
}
