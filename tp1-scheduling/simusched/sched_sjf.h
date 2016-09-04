#ifndef __SCHED_SJF__
#define __SCHED_SJF__

#include <vector>
#include <queue>
#include <algorithm>
#include "basesched.h"

using namespace std;

class SchedSJF : public SchedBase {
	public:
		SchedSJF(std::vector<int> argn);
		~SchedSJF();
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
		int cores;
};

#endif
