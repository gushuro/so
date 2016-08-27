#ifndef __SCHED_RR__
#define __SCHED_RR__

#include <vector>
#include <queue>
#include <algorithm>
#include "basesched.h"

using namespace std;

class SchedRR : public SchedBase {
	public:
		SchedRR(std::vector<int> argn);
		~SchedRR();
		virtual void initialize() {};
		virtual void load(int pid);
		virtual void unblock(int pid);
		virtual int tick(int cpu, const enum Motivo m);
	private:
/* llenar */
		queue<int> q;			// los pid de los procesos en ready.
		vector<int> quantums;	// los quantums de cada respectivo core
		vector<int> ticksleft;	// cuantos ticks le restan al proceso en cada core
};

#endif
