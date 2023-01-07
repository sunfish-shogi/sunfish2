/*
 * baseTable.h
 *
 *  Created on: 2012/11/03
 *      Author: ryosuke
 */

#ifndef BASETABLE_H_
#define BASETABLE_H_

namespace Table {
	template <class Entity>
	class BaseTable {
	private:
		unsigned size;
		unsigned mask;
		Entity* table;

	protected:
		Entity& _getEntity(Util::uint64 hash) {
			return table[hash&mask];
		}

		Entity& _getEntity(unsigned index) {
			return table[index];
		}

	public:
		static const unsigned DEF_BITS = 21;

		BaseTable(unsigned bits = DEF_BITS) : 
				size(0), table(NULL) {
			init(bits);
		}

		virtual ~BaseTable() {
			delete [] table;
		}

		void init(unsigned bits = 0) {
			unsigned newSize = 1 << bits;
			if (bits != 0 && size != newSize) {
				size = newSize;
				mask = size - 1;
				if (table != NULL) { delete[] table; }
				table = new Entity[size];
			} else {
				for (unsigned i = 0; i < size; i++) {
					table[i].init(i);
				}
			}
		}

		const Entity& getEntity(Util::uint64 hash) const {
			return table[hash&mask];
		}

		const Entity& getEntity(unsigned index) const {
			return table[index];
		}

		unsigned getSize() const {
			return size;
		}
	};
}

#endif // BASETABLE_H_
