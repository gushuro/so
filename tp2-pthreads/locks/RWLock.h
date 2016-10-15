#ifndef RWLock_h
#define RWLock_h
#include <iostream>

class RWLock {
	public:
		RWLock();
		void rlock();
		void wlock();
		void runlock();
		void wunlock();

	private:
		pthread_mutex_t m;
		pthread_cond_t turn;	/* Event: someone else's turn */

		int reading;			// cantidad de gente leyendo
		int writing;	// cant gente escribiendo (0 o 1)
		int writerswaiting;	// Gente esperando para escribir.
};

#endif
