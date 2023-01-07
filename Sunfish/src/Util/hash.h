/*
 * hash.h
 *
 *  Created on: 2013/01/13
 *      Author: ryosuke
 */

#ifndef HASH_H_
#define HASH_H_

#include "../Util/int.h"
#include "../Log/logger.h"

namespace Util {
	class Hash {
	private:
		Util::uint64* values;
		unsigned size;
		unsigned index;

		void create(const char* filename) {
			if (!read(filename)) {
				init();
				write(filename);
			}
		}

	public:
		static const char* DEFAULT_FILE_NAME;

		Hash(unsigned size) : size(size), index(0) {
			values = new Util::uint64[size];
			create(DEFAULT_FILE_NAME);
		}

		Hash(unsigned size, const char* filename) : size(size), index(0) {
			values = new Util::uint64[size];
			create(filename);
		}

		virtual ~Hash() {
			delete[] values;
		}

		void init();

		bool read(const char* filename);

		bool write(const char* filename);

		Util::uint64* get(unsigned requireSize) {
			if (index + requireSize > size) {
				return NULL;
			}
			unsigned prevIndex = index;
			index += requireSize;
			return values + prevIndex;
		}
	};
}

#endif // HASH_H_

