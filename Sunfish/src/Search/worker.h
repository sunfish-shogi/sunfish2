/*
 * worker.h
 *
 *  Created on: 2013/02/17
 *      Author: ryosuke
 */

#ifndef WORKER_H_
#define WORKER_H_

#include "searchCounter.h"
#define BOOST_THREAD_USE_LIB
#include <boost/thread.hpp>

namespace Search {
	class Tree;
	class Searcher;

	class Worker {
	private:
		boost::thread* thread;
		Searcher* psearcher;
		int worker;
		volatile int tree;
		volatile bool job;
		volatile bool shutdown;
		SearchCounter counter;

	public:
		Worker(int worker = 0, Searcher* psearcher = NULL) :
			psearcher(psearcher), worker(worker) {
		}

		void init(Searcher* psearcher, int worker) {
			this->psearcher = psearcher;
			this->worker = worker;
		}

		void start() {
			job = false;
			shutdown = false;
			thread = new boost::thread(boost::bind(
					&Worker::waitForJob, this, (Tree*)NULL));
		}

		void startCurrentThread(int tree) {
			this->tree = tree;
			shutdown = false;
			job = true;
		}

		void stop() {
			shutdown = true;
			thread->join();
			delete thread;
		}

		void waitForJob(Tree* suspend = NULL);

		void setJob(int tree) {
			this->tree = tree;
			job = true;
		}

		void unsetJob() {
			job = false;
		}

		void setTree(int tree) {
			this->tree = tree;
		}

		bool hasJob() const {
			return job;
		}

		bool mustShutdown() const {
			return shutdown;
		}

		SearchCounter& getCounter() {
			return counter;
		}
	};
}

#endif // WORKER_H_
