/*
 * paramAnalyzer.cpp
 *
 *  Created on: 2013/02/09
 *      Author: ryosuke
 */

#include "paramAnalyzer.h"
#include "../Util/tableString.h"
#include <sstream>

namespace Evaluates {
	using namespace Shogi;
	using namespace Util;

	template <class T, class U>
	std::string TempParamAnalyzer<T, U>::analyze() {
		std::ostringstream oss;

		{
			TableString table("", " = ", "");
			for (Piece piece = Piece::PAWN; piece <= Piece::DRAGON; piece.toNext()) {
				table.row() << piece.toStringNameOnly()
						<< param.getPiece(piece);
			}
			oss << table.get();
		}

		{
			const int num = KPP_ALL;
			U sum = 0;
			U max = U::MIN;
			U min = U::MAX;
			int nonZero = 0;
			for (int i = 0; i < num; i++) {
				U value = param.getKPP(i);
				sum += value;
				max = U::max(max, value);
				min = U::min(min, value);
				if (value != 0) { nonZero++; }
			}
			double ave = (double)sum / num;
			double nonZeroP = nonZero * 100.0 / num;
			TableString table("", " = ", "");
			table.row() << "KPP size" << num;
			table.row() << "KPP average" << ave;
			table.row() << "KPP max" << max;
			table.row() << "KPP min" << min;
			table.row() << "KPP nonZero" << nonZero;
			table.row() << "KPP nonZero(%)" << nonZeroP;
			oss << table.get();
		}

		{
			const int num = KKP_ALL;
			U sum = 0;
			U max = U::MIN;
			U min = U::MAX;
			int nonZero = 0;
			for (int i = 0; i < num; i++) {
				U value = param.getKKP(i);
				sum += value;
				max = U::max(max, value);
				min = U::min(min, value);
				if (value != 0) { nonZero++; }
			}
			double ave = (double)sum / num;
			double nonZeroP = nonZero * 100.0 / num;
			TableString table("", " = ", "");
			table.row() << "KKP size" << num;
			table.row() << "KKP average" << ave;
			table.row() << "KKP max" << max;
			table.row() << "KKP min" << min;
			table.row() << "KKP nonZero" << nonZero;
			table.row() << "KKP nonZero(%)" << nonZeroP;
			oss << table.get();
		}

		return oss.str();
	}
	template std::string TempParamAnalyzer<ValueS, ValueS>::analyze();
	template std::string TempParamAnalyzer<ValueD, ValueF>::analyze();
}
