#include "tasks.h"
#include <stdlib.h>
#include <algorithm>

using namespace std;

void TaskConsola(int pid, vector<int> params){
	int n = params[0];
	int bmin = params[1];
	int bmax = params[2];
	//srand(time(NULL));
	srand(314);
	//srand(314+pid);
	for(int i = 0; i < n; ++i){
		int r = ((double)rand()/RAND_MAX) * (bmax - bmin+1) + bmin;
		uso_IO(pid, r);
		uso_CPU(pid, 1);
	}
}

void TaskBatch(int pid, vector<int> params){
	int totalCpu = params[0];
	int cantBloqueos = params[1];
	vector<int> schedule = vector<int>(totalCpu, 0);
	for (int i = 0; i < totalCpu - cantBloqueos; ++i){
		schedule[i] = 1;
	}
	//srand(314+pid);
	srand(314);
	random_shuffle(schedule.begin(), schedule.end());
	for (int i = 0; i < totalCpu; ++i){
		if (schedule[i]) {
			uso_CPU(pid, 1);
		} else {
			uso_IO(pid, 2);
		}
	}
}


void TaskCPU(int pid, vector<int> params) { // params: n
	uso_CPU(pid, params[0] - 1); // Uso el CPU n milisegundos.
}

void TaskIO(int pid, vector<int> params) { // params: ms_pid, ms_io,
	uso_CPU(pid, params[0]); // Uso el CPU ms_pid milisegundos.
	uso_IO(pid, params[1]); // Uso IO ms_io milisegundos.
}

void TaskAlterno(int pid, vector<int> params) { // params: ms_pid, ms_io, ms_pid, ...
	for(int i = 0; i < (int)params.size(); i++) {
		if (i % 2 == 0) uso_CPU(pid, params[i] - 1);
		else uso_IO(pid, params[i]);
	}
}



void tasks_init(void) {
	/* Todos los tipos de tareas se deben registrar acá para poder ser usadas.
	 * El segundo parámetro indica la cantidad de parámetros que recibe la tarea
	 * como un vector de enteros, o -1 para una cantidad de parámetros variable. */
	register_task(TaskCPU, 1);
	register_task(TaskIO, 2);
	register_task(TaskAlterno, -1);
	register_task(TaskConsola, 3);
	register_task(TaskBatch, 2);
}
