/*
 * fileList.cpp
 *
 *  Created on: 2013/02/04
 *      Author: ryosuke
 */

#include "fileList.h"

#include "../Log/logger.h"

#ifdef WIN32
#	include <windows.h>
#else
#	include <dirent.h>
#	include <strings.h>
#endif

namespace Util {
	int FileList::enumerate(const char* directory, const char* extension) {
		std::string dir;
		std::string fname;
		std::string ext;
		unsigned int len;
		int num = 0;

#ifdef WIN32
		HANDLE hFind;
		WIN32_FIND_DATA fd;
#else
		DIR *pdir;
		struct dirent *pent;
#endif

		// ディレクトリ
		dir = directory;
		len = dir.length();

		// 拡張子
		ext = '.';
		ext += extension;

		// スラッシュを除去
		if( dir[len-1] == '/'
#ifdef WIN32
			|| dir[len-1] == '\\'
#endif
				) {
			dir = dir.substr(0, len-1);
		}

#ifdef WIN32
		// 拡張子に当てはまるファイルを列挙
		fname = dir + "\\*";
		if (1 < ext.length()) {
			fname += extension;
		}

		hFind = FindFirstFile(fname.c_str(), &fd);

		if( hFind == INVALID_HANDLE_VALUE ){
			Log::warning << "There are no files!..[" << fname << ']' << '\n';
			return 0;
		}
		dir += '\\';
#else
		// ディレクトリのオープン
		if ((pdir = opendir(dir.c_str())) == NULL) {
			Log::error << "Couldn't open the directory!..[" << dir << ']' << '\n';
			return 0;
		}
		dir += '/';
#endif

		for ( ; ; ) {
#ifdef WIN32
			fname = fd.cFileName;
#else
			if (NULL == (pent = readdir(pdir))) {
				closedir( pdir );
				break;
			}

			fname = pent->d_name;
#endif

			// ディレクトリ".", ".."の場合
			if (fname[fname.length()-1] == '.') {
				continue;
			}

#ifndef WIN32
			// 拡張子のチェック
			if (1 < (len = ext.length()) && (fname.length() <= len ||
					0 != strcasecmp(fname.substr(fname.length() - len).c_str(), ext.c_str()))) {
				continue;
			}
#endif

			// リストに追加
			flist.push_back(dir + fname);
			num++;

#ifdef WIN32
			// 次のファイル
			if (!FindNextFile( hFind, &fd)) {
				// ハンドルのクローズ
				FindClose( hFind );
				break;
			}
#endif
		}

		it = flist.begin();

		return num;
	}
}
