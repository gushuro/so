#include <vector>
#include <queue>
#include "sched_rr.h"
#include "basesched.h"
#include <iostream>

using namespace std;

SchedRR::SchedRR(vector<int> argn) {
	// Round robin recibe la cantidad de cores y sus cpu_quantum por parámetro
	quantums = vector<int> (argn[0]);
	ticksleft = vector<int> (argn[0]);
	cerr << "a0 " << argn[0] << " a1: "<< argn[1] << "   a2: " << argn[2] << endl;
	for (int i = 0; i < argn[0]; ++i) {
		quantums[i] = ticksleft[i] = argn[i+1]-1;

		cerr << quantums[i] << endl;
	}
}

SchedRR::~SchedRR() {
/* completar */
}


void SchedRR::load(int pid) {
/* completar */
	q.push(pid); // llegó una tarea nueva
}

void SchedRR::unblock(int pid) {
	q.push(pid); 	// sale de bloqueo, va a la cola de readys
/* completar */
}

int SchedRR::tick(int cpu, const enum Motivo m) {
/* completar */
	//cout << "pid: " << current_pid(cpu) << endl;
	if (m == EXIT){
		// Si el proceso terminó, lo saco de la cola y sigo.
		if (q.empty()) return IDLE_TASK;
		else{
			int sig = q.front(); q.pop();
			ticksleft[cpu] = quantums[cpu];
			return sig;
		}

	} else if (m == TICK){
		//Si estaba idle y hay tarea, correla
		if (current_pid(cpu) == IDLE_TASK && !q.empty()){

			int sig = q.front();
			q.pop();
			////cout << "error 1111" << endl;
			return sig;
		}
		// Si hay una tarea corriendo:
		if (ticksleft[cpu] == 0 && !q.empty()) {
			// si no le quedan ticks y hay otra esperando, al fondo.
			int sig = q.front();
			q.pop();
			q.push(current_pid(cpu));
			ticksleft[cpu] = quantums[cpu];
			//cout << "error 22222" << endl;
			return sig;
		} else if (ticksleft[cpu] == 0) {
			// no le quedan ticks pero la cola está vacía.
			return current_pid(cpu);
		} else {
			// le quedan ticks: descuento 1 y sigue el mismo.
			ticksleft[cpu]--;
			return current_pid(cpu);
		}
	} else {
		// block
		if (q.empty()) return IDLE_TASK;	// si no hay más tareas, IDLE.
		int sig = q.front();
		q.pop();
		ticksleft[cpu] = quantums[cpu];
		//cout << "error 3333" << endl;
		return sig;
	}
}
