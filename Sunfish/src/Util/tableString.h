/*
 * tableString.h
 *
 *  Created on: 2012/11/24
 *      Author: ryosuke
 */

#ifndef TABLESTRING_H_
#define TABLESTRING_H_

#include <string>
#include <sstream>
#include <iomanip>
#include <list>
#include <vector>

namespace Util{
	class TableString {
	private:
		std::string result;
		bool changed;
		typedef std::list<std::string> t_columns;
		typedef std::list<t_columns> t_rows;
		t_rows table;
		std::string prefix;
		std::string separator;
		std::string suffix;

		void maxLength(t_columns& columns, std::vector<int>& length) {
			t_columns::iterator ite;
			int index = 0;
			for (ite = columns.begin(); ite != columns.end(); ite++) {
				int len = (*ite).length();
				if (len > length[index]) {
					length[index] = len;
				}
				index++;
			}
		}

		void maxLength(t_rows& rows, std::vector<int>& length) {
			t_rows::iterator ite;
			for (ite = rows.begin(); ite != rows.end(); ite++) {
				maxLength(*ite, length);
			}
		}

	public:
		TableString(const char* separator) : prefix(""),
				separator(separator), suffix("") {
			changed = true;
		}

		TableString(const char* prefix, const char* separator,
				const char* suffix) : prefix(prefix),
				separator(separator), suffix(suffix) {
			changed = true;
		}

		TableString& row() {
			table.push_back(std::list<std::string>());
			changed = true;
			return *this;
		}

		template<class T>
		TableString& operator<<(T t) {
			std::ostringstream oss;
			oss << t;
			(*table.rbegin()).push_back(oss.str());
			changed = true;
			return *this;
		}

		int maxSize(t_rows& rows) {
			int maxSize = 0;
			t_rows::iterator ite;
			for (ite = rows.begin(); ite != rows.end(); ite++) {
				int size = ite->size();
				if (size > maxSize) {
					maxSize = size;
				}
			}
			return maxSize;
		}

		std::string get() {
			if (changed) {
				std::vector<int> length(maxSize(table));
				maxLength(table, length);
				std::ostringstream oss;
				oss << std::left;
				t_rows::iterator r;
				for (r = table.begin(); r != table.end(); r++) {
					t_columns::iterator c = (*r).begin();
					oss << prefix << std::setw(length[0]) << (*c);
					int index = 1;
					for (c++; c != (*r).end(); c++) {
						oss << separator << std::setw(length[index]) << (*c);
						index++;
					}
					oss << suffix << '\n';
				}
				result = oss.str();
				changed = false;
			}
			return result;
		}
	};
};

#endif // TABLESTRING_H_
