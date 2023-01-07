/*
 * aspWindow.h
 *
 *  Created on: 2012/08/19
 *      Author: ryosuke
 */

#ifndef ASP_WINDOW_H_
#define ASP_WINDOW_H_

#include "../Evaluates/value.h"

namespace Search {
	extern const int aspWind[2];
	extern const int ASP_MAX;

	// 段階的に広がる探索窓
	template<int sign>
	class AspWindow {
	private:
		int aspPhase;
		Evaluates::Value aspValue;

	public:
		AspWindow(int value) {
			init(value);
		}

		AspWindow() {
			init();
		}

		void init(int value) {
			aspValue = value;
			aspPhase = 0;
		}

		void init() {
			aspValue = 0; // 不要
			aspPhase = ASP_MAX;
		}

		bool next() {
			if (aspPhase == ASP_MAX) {
				return false;
			} else {
				aspPhase++;
				return true;
			}
		}

		Evaluates::Value getValue() const {
			if (aspPhase == ASP_MAX) {
				return Evaluates::Value::MAX * sign;
			} else {
				return aspValue + aspWind[aspPhase] * sign;
			}
		}

		operator int() const {
			return (int)getValue();
		}

		AspWindow<sign>& operator=(AspWindow<sign> asp) {
			aspPhase = asp.aspPhase;
			aspValue = asp.aspValue;
			return *this;
		}
	};
}

#endif // ASP_WINDOW_H_
