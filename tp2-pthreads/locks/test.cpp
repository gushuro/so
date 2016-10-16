#include <vector>
#include <cstdlib>
#include <unistd.h>
#include <pthread.h>
#include "RWLock.h"
#include <sys/types.h>
#include <cstring>

using namespace std;

RWLock rwlock = RWLock();

int contador;

uint64_t gettid() {
    pthread_t ptid = pthread_self();
    uint64_t threadId = 0;
    memcpy(&threadId, &ptid, std::min(sizeof(threadId), sizeof(ptid)));
    return threadId;
}

void* threadEscritor(void* args){
  auto tid = gettid();
  int contadorLocal;

  //printf("Thread %lu esta esperando para escribir \n", tid);
  rwlock.wlock();
    printf("Thread %lu tiene el recurso para escribir \n", tid);
    // el thread hace uso del mutex:
    sleep(1); // hace sleep para que no pase tan rapido
    contador++; // hace uso del recurso compartido
    contadorLocal = contador;
    printf("Thread %lu termino de escribir. Valor del contador: %i \n", tid, contadorLocal);
  rwlock.wunlock();

  return args;
}

void* threadLector(void* args){
  auto tid = gettid();

  int contadorLocal;

  //printf("Thread %lu esta esperando para leer \n", tid);
  rwlock.rlock();
    printf("Thread %lu tiene el recurso para leer \n", tid);
    // el thread hace uso del mutex:
    sleep(1); // hace sleep para que no pase tan rapido
    contadorLocal = contador; // hace uso del recurso compartido
    printf("Thread %lu termino de leer. Valor del contador: %i \n", tid, contadorLocal);
  rwlock.runlock();

  return args;
}

void test1(){
  // Caso dos lectores y un escritor a la vez
  contador = 0;
  //int n = rand() % 20 + 20; // random entre 20 y 40
  int n = 10;

  printf("Este test creará 10 threads.\n");

  pthread_t *threads;
  threads = (pthread_t*)calloc(n*3, sizeof(pthread_t));
  for(int i = 0; i < n*3; i=i+3){
    pthread_t* p1 = &threads[i];
    pthread_create(&threads[i], NULL, threadLector, p1);
    pthread_t* p2 = &threads[i+1];
    pthread_create(&threads[i+1], NULL, threadLector, p2);
    pthread_t* p3 = &threads[i+2];
    pthread_create(&threads[i+2], NULL, threadEscritor, p3);
  }
  pthread_exit(NULL); // necesario para que no termine antes que sus threads hijos
}


int main(int argc, char const *argv[]) {

  test1();

  return 0;
}
