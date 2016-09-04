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
	lastqueue[pid] = 0;
	vq[0].push(pid);
}

void SchedMFQ::unblock(int pid) {
	int where = max(lastqueue[pid]-1,0);
	vq[where].push(pid);
}

int SchedMFQ::tick(int core, const enum Motivo m) {
	if (m == EXIT){
		// Si el proceso termino, lo saco de la cola y sigo.
		return next(core);
	} else if (m == TICK) {
		// Si estoy en idle, dame el siguiente y actualiza:
		if (current_pid(core) == IDLE_TASK){
			return next(core);
		} else if (ticksleft[core] == 0){
				// Si hay una tarea corriendo:
			int act = current_pid(core);
			int where = min(lastqueue[act]+1, (int)(vq.size()-1));
			vq[where].push(act);	// Agoto sus ticks, para abajo.
			return next(core);
		} else{
			ticksleft[core]--;
			return current_pid(core);
		}
	} else {
		// BLOCK: Desalojo y meto al siguiente.
		return next(core);
	}
	//cerr << "TODO MAL" << endl;
	return 0;
}


int SchedMFQ::next(int core){
	// Me da el proximo pid. Lo saca de su cola y actualiza ticksleft y lastqueue
	for(int i = 0; i < (int)(vq.size()); i++){
		if (!(vq[i].empty())) {
			int res = vq[i].front();
			vq[i].pop();
			ticksleft[core] = quantums[i]-1;
			lastqueue[res] = i;
			return res;
		}
	}
	return IDLE_TASK;
}

