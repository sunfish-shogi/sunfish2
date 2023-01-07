/*
 * paramAnalyzer.h
 *
 *  Created on: 2013/02/09
 *      Author: ryosuke
 */

#ifndef PARAMANALYZER_H_
#define PARAMANALYZER_H_

#include "param.h"
#include <string>

namespace Evaluates {
	template <class T, class U>
	class TempParamAnalyzer {
	private:
		typedef TempParam<T, U> Type;

		const Type& param;

	public:
		TempParamAnalyzer(const Type& param) :
				param(param) {
		}

		std::string analyze();
	};

	typedef TempParamAnalyzer<ValueS, ValueS> ParamAnalyzer;
	typedef TempParamAnalyzer<ValueD, ValueF> GradientAnalyzer;
}

#endif // PARAMANALYZER_H_
