/*
 * learn.h
 *
 *  Created on: 2013/02/02
 *      Author: ryosuke
 */

#ifndef LEARN_H_
#define LEARN_H_

#include "pvWriter.h"
#include "learnConfig.h"
#include "../Evaluates/initializer.h"
#include "../Util/random.h"

namespace Learns {
	class Learn {
	private:
		struct LearnInfo {
			double loss;
			int samples;
		};

		static const double PENALTY;

		const char* configFilename;
		LearnConfig config;
		Evaluates::Param* pparam;
		LearnInfo info;
		Util::Random random;

		void beginAdjust();

		void endAdjust();

		void analyzeAllFiles();

		void analyzeFile(const char* path, PvWriter& writer);

		double loss(double x);

		double dLoss(double x);

		void generateGradient(Evaluates::Gradient& g);

		void adjustParam(const Evaluates::Gradient& g);

		Evaluates::ValueS adjustValue(Evaluates::ValueS prev, Evaluates::ValueF d);

	public:
		static const char* DEFAULT_CONFIG_FILE;

		Learn() {
			pparam = new Evaluates::Param();
			Evaluates::Initializer::apply(*pparam);
			pparam->read("evdata");
			configFilename = DEFAULT_CONFIG_FILE;
		}

		virtual ~Learn() {
			delete pparam;
		}

		void setConfigFile(const char* filename) {
			configFilename = filename;
		}

		void execute();
	};
}

#endif // LEARN_H_
