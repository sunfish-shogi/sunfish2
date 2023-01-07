/*
 * learnConfig.h
 *
 *  Created on: 2013/02/02
 *      Author: ryosuke
 */

#ifndef LEARNCONFIG_H_
#define LEARNCONFIG_H_

#include "../Util/tableString.h"
#include "../Config/configure.h"

namespace Learns {
	class LearnConfig : public Configures::Configure {
	private:
		Configures::ConfigItem items[4];

		std::string dir; // 棋譜の保存場所
		int ups; // 1 step あたりの更新回数
		int steps; // 反復回数
		int depth; // 探索深さ

	protected:
		Configures::ConfigItem* itemList() {
			return items;
		}

		int itemSize() {
			return sizeof(items)/sizeof(items[0]);
		}

	public:
		LearnConfig(const char* filename = NULL);

		std::string getDir() const {
			// TODO: 共通化
			if (dir.length() > 0 && dir[dir.length()-1] == '/') {
				return dir;
			} else {
				return dir + '/';
			}
		}

		int getUPS() const {
			return ups;
		}

		int getSteps() const {
			return steps;
		}

		int getDepth() const {
			return depth;
		}

		std::string toString() const {
			Util::TableString table("* ", " :", "");
			table.row() << "DIR" << getDir();
			table.row() << "UPS" << getUPS();
			table.row() << "STEPS" << getSteps();
			table.row() << "DEPTH" << getDepth();
			return table.get();
		}
	};
}

#endif // LEARNCONFIG_H_
