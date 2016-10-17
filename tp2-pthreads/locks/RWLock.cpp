#include "RWLock.h"

RWLock :: RWLock() {
	reading = 0;
	writing = 0;
	writerswaiting = 0;
	m = PTHREAD_MUTEX_INITIALIZER;
	turn = PTHREAD_COND_INITIALIZER;

	puedoPasar = true;
	antesalaM = PTHREAD_MUTEX_INITIALIZER;
	antesalaCv = PTHREAD_COND_INITIALIZER;	

}

void RWLock :: rlock() {
	// Espero a que no haya escritores
	pthread_mutex_lock(&antesalaM);
		while(!puedoPasar) 
			pthread_cond_wait(&antesalaCv, &antesalaM);
	pthread_mutex_unlock(&antesalaM);

	pthread_mutex_lock(&m);
		// No hay escritor esperando
		// espero mientras alguien escribiendo
		while (!(writing == 0)) {
			pthread_cond_wait(&turn, &m);
		}
		// sumo a los lectores
		reading++;
	pthread_mutex_unlock(&m);

}

void RWLock :: wlock() {
	pthread_mutex_lock(&antesalaM);
		while(!puedoPasar) 
			pthread_cond_wait(&antesalaCv, &antesalaM);
		puedoPasar = false; // no dejo pasar a nadie mas
	pthread_mutex_unlock(&antesalaM);

	pthread_mutex_lock(&m);
		writerswaiting++;
		while (!(reading == 0 && writing == 0)) {
			pthread_cond_wait(&turn, &m);
		}
		// escribo
		writerswaiting--;
		writing = 1;
	pthread_mutex_unlock(&m);
}

void RWLock :: runlock() {
	pthread_mutex_lock(&m);
		reading--;
		if (reading == 0){
			pthread_cond_broadcast(&turn);
		}
	pthread_mutex_unlock(&m);
}

void RWLock :: wunlock() {
	pthread_mutex_lock(&antesalaM);
	    puedoPasar = true;
		pthread_cond_broadcast(&antesalaCv);
	pthread_mutex_unlock(&antesalaM);

	pthread_mutex_lock(&m);
		writing = 0;
		pthread_cond_broadcast(&turn);
	pthread_mutex_unlock(&m);
}
