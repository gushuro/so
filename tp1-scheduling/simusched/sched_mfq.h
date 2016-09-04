#ifndef __SCHED_MFQ__
#define __SCHED_MFQ__

#include <vector>
#include <queue>
#include <unordered_map>

#include "basesched.h"

using namespace std;
class SchedMFQ : public SchedBase {
	public:
		SchedMFQ(std::vector<int> argn);
		~SchedMFQ();
		virtual void initialize() {};
		virtual void load(int pid);
		virtual void unblock(int pid);
		virtual int tick(int n, const enum Motivo m);
		virtual int next();
	private:
/* llenar */
		vector<queue<int> > vq;		// los pid de los procesos en ready.
		vector<int> quantums;		// los quantums de cada respectiva cola
		vector<int> ticksleft;		// cuantos ticks le restan al proceso en cada core
		unordered_map<int,int> lastqueue;	// la última cola en la que estuvo cada proceso

};

#endif
