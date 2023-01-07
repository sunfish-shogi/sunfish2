/*
 * fileList.h
 *
 *  Created on: 2013/02/04
 *      Author: ryosuke
 */

#ifndef FILELIST_H_
#define FILELIST_H_

#include <list>
#include <string>

namespace Util {
	class FileList{
	private:
		std::list<std::string> flist;

		std::list<std::string>::iterator it;

		void push(const std::string& fname) {
			flist.push_back(fname);
		}

	public:
		FileList(){
			it = flist.begin();
		}

		virtual ~FileList(){
		}

		int enumerate( const char* directory, const char* extension );

		void clear(){
			flist.clear();
			it = flist.begin();
		}

		void begin() {
			it = flist.begin();
		}

		bool isEnd() {
			return it == flist.end();
		}

		const std::string& pop() {
			const std::string& ret = (*it);
			it++;
			return ret;
		}

		int size() {
			return flist.size();
		}
	};
}

#endif // FILELIST_H_
