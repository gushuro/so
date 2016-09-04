#include "sched_rsjf.h"
#include <iostream>

using namespace std;

SchedRSJF::SchedRSJF(vector<int> argn) {
        // Recibe por parámetro la cantidad de cores, sus cpu_quantum y los tiempos de ejecución de cada tarea en el lote
	quantums = vector<int> (argn[0]); // quantums por core
	ticksleft = vector<int> (argn[0]); // ticks que le quedan al proceso en ese core
	timeleft = vector<int> (argn.size()-1-argn[0]);  // tiempo que le queda al proceso en total
	for (int i = 0; i < argn[0]; ++i) {
		quantums[i] = ticksleft[i] = argn[i+1]-1;
	}
	for (int i = 0; i < argn.size()-1-argn[0]; ++i) {
		timeleft[i] = argn[i+argn[0]+1];
	}
}

SchedRSJF::~SchedRSJF() {
/* llenar */
}

void SchedRSJF::load(int pid) {
	proc p;
	p.pid = pid;
	p.time = timeleft[pid];
	q.push(p);
}

void SchedRSJF::unblock(int pid) {
/* llenar */
}

int SchedRSJF::tick(int core, const enum Motivo m) {
/* llenar */
	if (m == EXIT){
		// Si el proceso terminó, lo saco de la cola y sigo.
		if (q.empty()) return IDLE_TASK;
		else{
			int sig = q.top().pid; q.pop();
			ticksleft[core] = quantums[core];
			return sig;
		}

	} else if (m == TICK){
		//Si estaba idle y hay tarea, correla
		if (current_pid(core) == IDLE_TASK && !q.empty()){

			int sig = q.top().pid;
			q.pop();
			return sig;
		}
		// Si hay una tarea corriendo:
		if (ticksleft[core] == 0 && !q.empty()) {
			// si no le quedan ticks, lo meto en la cola y saco el minimo (puede ser el mismo).
			proc p;
			p.pid = current_pid(core);
			p.time = --timeleft[current_pid(core)];
			q.push(p);
			int sig = q.top().pid;
			q.pop();
			ticksleft[core] = quantums[core];
			return sig;
		} else if (ticksleft[core] == 0) {
			// no le quedan ticks pero la cola está vacía.
			return current_pid(core);
		} else {
			// le quedan ticks: descuento 1 y sigue el mismo.
			ticksleft[core]--;
			timeleft[current_pid(core)]--;
			return current_pid(core);
		}
	} else {
		// block
		if (q.empty()) return IDLE_TASK;	// si no hay más tareas, IDLE.
		int sig = q.top().pid;
		q.pop();
		ticksleft[core] = quantums[core];
		//cout << "error 3333" << endl;
		return sig;
	}
}
