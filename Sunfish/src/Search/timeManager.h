/*
 * timeManager.h
 *
 *  Created on: 2013/03/02
 *      Author: ryosuke
 */

#ifndef TIMEMANAGER_H_
#define TIMEMANAGER_H_

#include "../Evaluates/value.h"

namespace Search {
	class TimeManager {
	private:
		bool limitEnable;
		double limitSeconds;
		int depth;
		struct {
			Evaluates::Value first;
			Evaluates::Value second;
		} stack[64];

		void init();

	public:
		TimeManager(bool limitEnable, double limitSeconds) {
			this->limitEnable = limitEnable;
			this->limitSeconds = limitSeconds;
			depth = 0;
			init();
		}

		void next() {
			depth++;
			init();
		}

		void value(Evaluates::Value value) {
			if (value > stack[depth].first) {
				stack[depth].second = stack[depth].first;
				stack[depth].first = value;
			} else if (value > stack[depth].second) {
				stack[depth].second = value;
			}
		}

		bool closure(double elapsed);
	};
}

#endif // TIMEMANAGER_H_
