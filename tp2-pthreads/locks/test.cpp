#include <vector>
#include <cstdlib>
#include <unistd.h>
#include <pthread.h>
#include "RWLock.h"
#include <sys/types.h>


using namespace std;

RWLock rwlock = RWLock();

int contador;

void* threadEscritor(void* args){
  auto tid = gettid();
  int contadorLocal;


  cout << "Thread Num " << tid << " - esta esperando para escribir" << endl;
  rwlock.wlock();
    cout << "Thread Num " << tid << " - tiene el recurso para escribir" << endl;
    // el thread hace uso del mutex:
    sleep(1); // hace sleep para que no pase tan rapido
    contador++; // hace uso del recurso compartido
    contadorLocal = contador;
    cout << "Thread Num " << tid << " - termino de escribir. Valor del contador: " << contadorLocal << endl;
  rwlock.wunlock();
}

void* threadLector(void* args){
  auto tid = gettid();

  int contadorLocal;

  rwlock.rlock();
    cout << "Thread Num " << tid << " - esta esperando para leer" << endl;
    // el thread hace uso del mutex:
    sleep(1); // hace sleep para que no pase tan rapido
    contadorLocal = contador; // hace uso del recurso compartido
    cout << "Thread Num " << tid << " - termino de leer. Valor del contador: " << contadorLocal << endl;
  rwlock.runlock();
}

void* test1(void* argv){
  // Caso dos lectores y un escritor a la vez
  contador = 0;
  int n = rand() % 20 + 20; // random entre 20 y 40

  vector<int> threads = vector<int>(n*3);
  for(int i = 0; i < n*3; i=i+3){
    pthread_create(&threads[i], NULL, threadLector, (void*)&threads[i]);
    pthread_create(&threads[i+1], NULL, threadLector, (void*)&threads[i+1]);
    pthread_create(&threads[i+2], NULL, threadEscritor, (void*)&threads[i+2]);
  }
}

void* rwInteger(void* argv){
  unsigned int id = pthread_self();
  int random = rand() % 3;
    if(!random){
      rwlock.wlock();
      cout << "El thread " << id << " quiere escribir" << endl;
      sleep(1);
      cout << "El thread " << id << " sale" << endl;
      rwlock.wunlock();
    } else {
      rwlock.rlock();
      cout << "El thread " << id << " quiere leer la variable compartido" << endl;
      sleep(1);
      cout << "El thread " << id << " sale" << endl;
      rwlock.runlock();
    }
  pthread_exit(NULL);
  return argv;
}


int main(int argc, char const *argv[]) {

  test1();
  /*pthread_t t1[NUMBER_THREADS];

  for(int i = 0 ; i < NUMBER_THREADS; i++) 
    pthread_create(&t1[i], NULL, &rwInteger, NULL) ;

  cout << "Todos los threads creados" << endl;
  for(int i = 0 ; i < NUMBER_THREADS; i++) 
    pthread_join(t1[i], NULL);*/

  return 0;
}
