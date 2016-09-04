#include <vector>
#include <queue>
#include <iostream>
#include <unordered_map>
#include "sched_mfq.h"
#include "basesched.h"

using namespace std;
#define DBG(x) cerr << #x << " = " << (x) << endl

SchedMFQ::SchedMFQ(vector<int> argn) {
	// MFQ recibe los quantums por parametro

// renames de parámetros

	int cores = argn[0];		// cantidad de cores
	int n = argn.size()-1;		// cantidad de colas

// inicializo estructuras

	ticksleft = vector<int> (cores, 0);		// ticks restantes en cada core
	vq = vector<queue<int> > (n, queue<int>() );	// vector de n colas, todas vacias
	lastqueue = unordered_map<int,int>();

	quantums = vector<int> (n);		// los qi
	for (int i = 0; i < n; ++i) {
		quantums[i] = argn[i+1];
	}
}

SchedMFQ::~SchedMFQ() {
/* llenar */
}

void SchedMFQ::load(int pid) {
	pair<int,int> p(pid,0);
	lastqueue.insert(p);
	vq[0].push(pid);
}

void SchedMFQ::unblock(int pid) {
/* llenar */
	int where = max(lastqueue[pid]-1,0);
	vq[where].push(pid);
}

int SchedMFQ::tick(int core, const enum Motivo m) {
/* llenar */
	if (m == EXIT){
		// Si el proceso termino, lo saco de la cola y sigo.
		cerr << "exited process " << current_pid(core) << endl;
		return next(core, true);
	} else if (m == TICK) {

		// Si estoy en idle, dame el siguiente y actualiza:
		if (current_pid(core) == IDLE_TASK){
			return next(core,true);
		}

		// Si hay una tarea corriendo:
		if (ticksleft[core] == 0){
			int act = current_pid(core);
			int where = min(lastqueue[act]+1, (int)vq.size());
			vq[where].push(act);	// Agoto sus ticks, para abajo.
			return next(core, true);
		} else{
			ticksleft[core]--;
			return current_pid(core);
		}
	} else {
		// BLOCK: Desalojo y meto al siguiente.
		return next(core, true);
	}
	return 0;
}


int SchedMFQ::next(int core, bool update){
	// Me da el proximo pid. Si update es true, lo saca de su cola y actualiza ticksleft y lastqueue
	for(int i = 0; i < (int)(vq.size()); i++){
		if (!(vq[i].empty())) {
			int res = vq[i].front();
			if (update) {
				vq[i].pop();
				ticksleft[core] = quantums[i];
				lastqueue[res] = i;
			}
			DBG(res);
			return res;
		}
	}
	//DBG(IDLE_TASK);
	return IDLE_TASK;
}

