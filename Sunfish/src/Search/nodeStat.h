/*
 * nodeStat.h
 *
 *  Created on: 2013/01/13
 *      Author: ryosuke
 */

#ifndef NODESTAT_H_
#define NODESTAT_H_ 

namespace Search {
	class NodeStat {
	private:
		// 項目の数が変わった場合 => Table::TTEntity::e
		enum {
			NULL_MOVE = 1 << 0,
			RECAPTURE = 1 << 1,
			MATE      = 1 << 2,
			HASH_CUT  = 1 << 3,

			DEF_STAT = NULL_MOVE | RECAPTURE |
					MATE | HASH_CUT,
		};

		unsigned stat;

		void set(unsigned flag) {
			stat |= flag;
		}

		void unset(unsigned flag) {
			stat &= ~flag;
		}

		bool is(unsigned flag) const {
			return stat & flag;
		}

	public:
		NodeStat() : stat(DEF_STAT) {
		}

		NodeStat(unsigned stat) : stat(stat) {
		}

		NodeStat(const NodeStat& nodeStat) : stat(nodeStat.stat) {
		}

		NodeStat& setNullMove() {
			set(NULL_MOVE);
			return *this;
		}

		NodeStat& unsetNullMove() {
			unset(NULL_MOVE);
			return *this;
		}

		bool isNullMove() const {
			return is(NULL_MOVE);
		}

		NodeStat& setRecapture() {
			set(RECAPTURE);
			return *this;
		}

		NodeStat& unsetRecapture() {
			unset(RECAPTURE);
			return *this;
		}

		bool isRecapture() const {
			return is(RECAPTURE);
		}

		NodeStat& setMate() {
			set(MATE);
			return *this;
		}

		NodeStat& unsetMate() {
			unset(MATE);
			return *this;
		}

		bool isMate() const {
			return is(MATE);
		}

		NodeStat& setHashCut() {
			set(HASH_CUT);
			return *this;
		}

		NodeStat& unsetHashCut() {
			unset(HASH_CUT);
			return *this;
		}

		bool isHashCut() const {
			return is(HASH_CUT);
		}

		operator unsigned() const {
			return stat;
		}
	};
}

#endif // NODESTAT_H_ 
