#ifndef __SCHED_RSJF__
#define __SCHED_RSJF__

#include <vector>
#include <queue>
#include <algorithm>
#include "basesched.h"

using namespace std;

class SchedRSJF : public SchedBase {
	public:
		SchedRSJF(std::vector<int> argn);
        ~SchedRSJF();
		virtual void initialize() {};
		virtual void load(int pid);
		virtual void unblock(int pid);
		virtual int tick(int cpu, const enum Motivo m);	
	private:
/* llenar */
	struct proc {
		int pid;
		int time;
	};
	vector<int> timeleft;
	struct comp{
		bool operator()(proc i, proc j){
			return i.time > j.time;
		}
	};
	priority_queue<proc, vector<proc>, comp> q;
	vector<int> quantums;	// los quantums de cada respectivo core
	vector<int> ticksleft;	// cuantos ticks le restan al proceso en cada core

};

#endif
