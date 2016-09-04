#include <vector>
#include <queue>
#include <iostream>
#include "sched_sjf.h"

using namespace std;
#define DBG(x) cerr << #x << " = " << (x) << endl

SchedSJF::SchedSJF(vector<int> argn) {
		// Recibe la cantidad de cores y los tiempos de ejecución de cada tarea en el lote
/* llenar */
	timeleft = vector<int> (argn.size()-1);
	//cerr << "a0 " << argn[0] << " a1: "<< argn[1] << "   a2: " << argn[2] << endl;
	for (unsigned int i = 0; i < argn.size()-1; ++i) {
		timeleft[i] = argn[i+1];
	}
}

SchedSJF::~SchedSJF() {
/* llenar */
}

void SchedSJF::load(int pid) {
/* llenar */
	proc p;
	p.pid = pid;
	p.time = timeleft[pid];
	q.push(p);
}

void SchedSJF::unblock(int pid) {
/* llenar */
}

int SchedSJF::tick(int cpu, const enum Motivo m) {
/* llenar */
	if (m == EXIT) {
		// Si el pid actual terminó, sigue el próximo.
		if (q.empty()) return IDLE_TASK;
		else {
			int sig = q.top().pid; q.pop();
			return sig;
		}
	} else {
		// Siempre sigue el pid actual mientras no termine.
		if (current_pid(cpu) == IDLE_TASK && !q.empty()) {
			int sig = q.top().pid; q.pop();
			return sig;
		} else {
			return current_pid(cpu);
		}
	}
	return 0;
}
