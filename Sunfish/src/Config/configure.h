/*
 * configure.h
 *
 *  Created on: 2013/02/03
 *      Author: ryosuke
 */

#ifndef CONFIGURE_H_
#define CONFIGURE_H_

#include <string>
#include <typeinfo>

namespace Configures {
	enum ItemType {
		BOOL,
		INTEGER,
		STRING,
	};

	struct ConfigItem {
		const char* name;
		ItemType type;
		void* data;
		const char* defaultValue;
		ConfigItem() {
		}
		ConfigItem(const char* name, ItemType type,
				void* data, const char* defaultValue) {
			this->name = name;
			this->type = type;
			this->data = data;
			this->defaultValue = defaultValue;
		}
	};

	class Configure {
	private:
		static const int LINE_BUFFER_SIZE = 1024;

		void init();

		bool readLine(const char* line);

		bool convert(ConfigItem& item, const std::string& str);

	protected:
		virtual ConfigItem* itemList() = 0;

		virtual int itemSize() = 0;

	public:
		bool read(const char* filename);

		std::string toString();

	};
}

#endif // CONFIGURE_H_
