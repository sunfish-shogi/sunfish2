/*
 * pruningExpr.h
 *
 *  Created on: 2013/04/13
 *      Author: ryosuke
 */

#ifndef PRUNINGEXPR_H_
#define PRUNINGEXPR_H_

#ifdef PRUN_EXPR

#include "../Util/int.h"

namespace Search {
	class PruningExpr {
	private:
		static PruningExpr ins;
		static const int MIN_VAL = 0;
		static const int MAX_VAL = 100;
		static const int MAX_DEP = 32;
		static const int MAX_REC = 100;
		Util::uint64 fut_suc[MAX_DEP+1][MAX_VAL-MIN_VAL+1][MAX_REC+1];
		Util::uint64 fut_suc_k[MAX_DEP+1][MAX_VAL-MIN_VAL+1][MAX_REC+1];
		Util::uint64 ext_fut_suc[MAX_DEP+1][MAX_VAL-MIN_VAL+1][MAX_REC+1];
		Util::uint64 razor_suc[MAX_DEP+1][MAX_VAL-MIN_VAL+1][MAX_REC+1];
		Util::uint64 stat_suc[MAX_DEP+1][MAX_VAL-MIN_VAL+1][MAX_REC+1];
		Util::uint64 count_suc[MAX_DEP+1][MAX_VAL-MIN_VAL+1][MAX_REC+1];
		Util::uint64 null_suc[MAX_DEP+1][MAX_REC+1];
		Util::uint64 fut_err[MAX_DEP+1][MAX_VAL-MIN_VAL+1][MAX_REC+1];
		Util::uint64 fut_err_k[MAX_DEP+1][MAX_VAL-MIN_VAL+1][MAX_REC+1];
		Util::uint64 ext_fut_err[MAX_DEP+1][MAX_VAL-MIN_VAL+1][MAX_REC+1];
		Util::uint64 razor_err[MAX_DEP+1][MAX_VAL-MIN_VAL+1][MAX_REC+1];
		Util::uint64 stat_err[MAX_DEP+1][MAX_VAL-MIN_VAL+1][MAX_REC+1];
		Util::uint64 count_err[MAX_DEP+1][MAX_VAL-MIN_VAL+1][MAX_REC+1];
		Util::uint64 null_err[MAX_DEP+1][MAX_REC+1];
		PruningExpr();

		static int depth(int dep);

		static int value(int val);

		static int count(int cnt);

		static int record(int rec);

		static void summarize(Util::uint64 suc[], Util::uint64 err[], int size,
				double& average, double& deviation, double& rateAll, double& max, double& min);

		static void print(const char* name,
				Util::uint64 suc[][MAX_VAL-MIN_VAL+1][MAX_REC+1],
				Util::uint64 err[][MAX_VAL-MIN_VAL+1][MAX_REC+1]);

		static void print(const char* name,
				Util::uint64 suc[][MAX_REC+1],
				Util::uint64 err[][MAX_REC+1]);

	public:
		static void success1(int dep, int rec, bool isFut, int fut, bool isKing,
			bool isExtFut, int extFut, bool isCount, int cnt);

		static void error1(int dep, int rec, bool isFut, int fut, bool isKing,
			bool isExtFut, int extFut, bool isCount, int cnt);

		static void success2(int dep, int rec, bool isRazor, int razor);

		static void error2(int dep, int rec, bool isRazor, int razor);

		static void success3(int dep, int rec, bool isStat, int stat, bool isNull);

		static void error3(int dep, int rec, bool isStat, int stat, bool isNull);

		static void print();

	};
}

#endif // PRUN_EXPR

#endif // PRUNINGEXPR_H_
