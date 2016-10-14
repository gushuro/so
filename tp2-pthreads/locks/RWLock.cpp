#include "RWLock.h"

RWLock :: RWLock() {
	reading = 0;
	writing = 0;
	writerswaiting = 0; // ver; -1
	m = PTHREAD_MUTEX_INITIALIZER;
	turn = PTHREAD_COND_INITIALIZER;
}

void RWLock :: rlock() {
	pthread_mutex_lock(&m);
	if (writerswaiting != 0) {
		while (writing != 0) {
			// si hay uno escribiendo, espero
			pthread_cond_wait(&turn, &m);
		}
	}
	reading++;
	pthread_mutex_unlock(&m);

}

void RWLock :: wlock() {
	pthread_mutex_lock(&m);
	while (reading !=0 && writing != 0) {
		pthread_cond_wait(&turn, &m);
	}
	// escribo
	writing = 1;
	pthread_mutex_unlock(&m);
}

void RWLock :: runlock() {
	pthread_mutex_lock(&m);
	reading--;
	if (reading == 0){
		pthread_cond_broadcast(&turn);
	}
	thread_mutex_unlock(&m);
}

void RWLock :: wunlock() {
	pthread_mutex_lock(&m);
	writing = 0;
	pthread_cond_broadcast(&turn);
	pthread_mutex_unlock(&m);
}
