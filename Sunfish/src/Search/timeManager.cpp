/*
 * timeManager.cpp
 *
 *  Created on: 2013/03/02
 *      Author: ryosuke
 */

#include "timeManager.h"

#define EASY_DEPTH					5

namespace Search {
	using namespace Evaluates;

	void TimeManager::init() {
		stack[depth].first = Value::MIN;
		stack[depth].second = Value::MIN;
	}

	bool TimeManager::closure(double elapsed) {
		if (depth == 0) {
			return 0;
		}

		Value prev1st = stack[depth-1].first;
		Value first = stack[depth].first;
		Value second = stack[depth].second;
		Value diff = first - second;

		if (depth >= EASY_DEPTH && diff >= 500) {
			int easyDepth = depth - EASY_DEPTH;
			Value easy1st = stack[easyDepth].first;
			Value easy2nd = stack[easyDepth].second;
			Value easyDiff = easy1st - easy2nd;
			if (easyDiff >= 800) {
				if (first >= easy1st - 185 &&
						first <= easy1st + 350 &&
						Value::abs(first) <= 420) {
					return true;
				}
			} else if (easyDiff >= 600) {
				if (first >= easy1st - 90 &&
						first <= easy1st + 185 &&
						Value::abs(first) <= 185) {
					return true;
				}
			}
		}

		double baseSeconds = limitEnable ?  limitSeconds / 10.0 : 5;
		if (elapsed >= baseSeconds) {
			if (first >= prev1st && first <= prev1st + 128) {
				return true;
			} else if (diff >= 400 && first >= prev1st - 80) {
				return true;
			}
		}

		return false;
	}
}
