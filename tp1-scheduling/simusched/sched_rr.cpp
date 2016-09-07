#include <vector>
#include <queue>
#include "sched_rr.h"
#include "basesched.h"

using namespace std;

SchedRR::SchedRR(vector<int> argn) {
	// Round robin recibe la cantidad de cores y sus cpu_quantum por parámetro
	quantums = vector<int> (argn[0]);
	ticksleft = vector<int> (argn[0]);
	for (int i = 0; i < argn[0]; ++i) {
		quantums[i] = ticksleft[i] = argn[i+1]-1;
	}
}

SchedRR::~SchedRR() {
}


void SchedRR::load(int pid) {
	q.push(pid); // llegó una tarea nueva
}

void SchedRR::unblock(int pid) {
	q.push(pid); 	// sale de bloqueo, va a la cola de readys
}

int SchedRR::tick(int cpu, const enum Motivo m) {
	if (m == EXIT){
		// Si el anterior proceso terminó, reviso si hay alguna esperando para ejecutar
		if (q.empty()) return IDLE_TASK; // si no hay ninguna, ejecuto IDLE
		else{
			// Si hay alguna otra, hay que popearla y ejecutarla
			int sig = q.front(); q.pop();
			ticksleft[cpu] = quantums[cpu];
			return sig;
		}

	} else if (m == TICK){
		//Si estaba idle y hay tarea, correla
		if (current_pid(cpu) == IDLE_TASK && !q.empty()){

			int sig = q.front();
			q.pop();
			return sig;
		}
		if (current_pid(cpu) == IDLE_TASK){
			return IDLE_TASK;
		}
		// Si hay una tarea corriendo:
		if (ticksleft[cpu] == 0 && !q.empty()) {
			// si no le quedan ticks y hay otra esperando, al fondo.
			int sig = q.front();
			q.pop();
			q.push(current_pid(cpu));
			ticksleft[cpu] = quantums[cpu];
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
		return sig;
	}
}
