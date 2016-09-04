#include <vector>
#include <queue>
#include <iostream>
#include <unordered_map>
#include "sched_mfq.h"
#include "basesched.h"

using namespace std;
#define DBG(x) cerr << #x << " = " << (x) << endl

SchedMFQ::SchedMFQ(vector<int> argn) {
	// MFQ recibe los quantums por parámetro
/* llenar */
	int cores = argn[0];		// cantidad de cores
	int n = argn.size()-1;		// cantidad de colas
	ticksleft = vector<int> (cores, 0);		// ticks restantes en cada core
	vq = vector<queue<int> > (n, queue<int>() );	// vector de n colas, todas vacías
	quantums = vector<int> (n);				// los qi
	lastqueue = unordered_map<int,int>();
	//cerr << "a0 " << argn[0] << " a1: "<< argn[1] << "   a2: " << argn[2] << endl;
	for (int i = 0; i < n; ++i) {
		quantums[i] = argn[i+1];

		//cerr << quantums[i] << endl;
	}
}

SchedMFQ::~SchedMFQ() {
/* llenar */
}

void SchedMFQ::load(int pid) {
/* llenar */
}

void SchedMFQ::unblock(int pid) {
/* llenar */
}

int SchedMFQ::tick(int core, const enum Motivo m) {
/* llenar */

	return 0;
}


int SchedMFQ::next(){
	for(auto it = vq.begin(); it != vq.end(); it++){
		if (!(it->empty())) {
			int res = it->front();
			it->pop();
			return res;
		}
	}
	return IDLE_TASK;
}
