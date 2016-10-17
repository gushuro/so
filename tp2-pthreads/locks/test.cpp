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

void menu(){
  printf("\n████████╗███████╗███████╗████████╗    ██████╗ ██╗    ██╗██╗      ██████╗  ██████╗██╗  ██╗ \n");
  printf("╚══██╔══╝██╔════╝██╔════╝╚══██╔══╝    ██╔══██╗██║    ██║██║     ██╔═══██╗██╔════╝██║ ██╔╝ \n");
  printf("   ██║   █████╗  ███████╗   ██║       ██████╔╝██║ █╗ ██║██║     ██║   ██║██║     █████╔╝  \n");
  printf("   ██║   ██╔══╝  ╚════██║   ██║       ██╔══██╗██║███╗██║██║     ██║   ██║██║     ██╔═██╗  \n");
  printf("   ██║   ███████╗███████║   ██║       ██║  ██║╚███╔███╔╝███████╗╚██████╔╝╚██████╗██║  ██╗ \n");
  printf("   ╚═╝   ╚══════╝╚══════╝   ╚═╝       ╚═╝  ╚═╝ ╚══╝╚══╝ ╚══════╝ ╚═════╝  ╚═════╝╚═╝  ╚═╝ \n\n");

  printf("Para revisar el correcto funcionamiento del RWLock, ejecutamos distintos casos de threads lectores y escritores \n");
  printf("que comparten un mismo recurso: Un contador que comienza en 0. Los escritores lo incrementan en 1,  \n");
  printf("y los lectores leen su contenido y lo imprimen en pantalla \n \n");

  printf("Tests posibles a ejecutar: \n\n");
  printf("________ CASOS BORDES ______________________ \n");
  printf("1 - 20 threads de lectura \n");
  printf("2 - 20 threads de escritura \n\n");

  printf("________ CASO INTERCALADOS _________________ \n");
  printf("3 - 20 lectores y 10 escritores, intercalados primero 2 lectores y luego 1 escritor \n");
  printf("4 - 10 lectores y 20 escritores, intercalados primero 2 escritores y luego 1 lector \n\n");

  printf("________ CASOS DE POSIBLE INANICIÓN ________ \n");
  printf("5 - 5 escritores, luego 1 lector y después 14 escritores \n");
  printf("6 - 5 lectores, luego 1 escritor y después 14 lectores \n\n");

  printf("________ CASOS ALEATORIOS __________________ \n");
  printf("7 - 20 threads de tipo aleatorio \n");
  printf("8 - 20 threads de tipo aleatorio, y duracion aleatoria \n\n");
  

  printf("Ingrese número de test a ejecutar (Q para salir, H para mostrar este menú): ");
}

void* threadEscritor(void* args){
  auto tid = gettid();
  int contadorLocal;

  printf("Thread %lu esta esperando para escribir \n", tid);
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

  printf("Thread %lu esta esperando para leer \n", tid);
  rwlock.rlock();
    printf("Thread %lu tiene el recurso para leer \n", tid);
    // el thread hace uso del mutex:
    sleep(1); // hace sleep para que no pase tan rapido
    contadorLocal = contador; // hace uso del recurso compartido
    printf("Thread %lu termino de leer. Valor del contador: %i \n", tid, contadorLocal);
  rwlock.runlock();

  return args;
}

void* threadEscritorDuracion2(void* args){
  auto tid = gettid();
  int contadorLocal;

  printf("Thread %lu esta esperando para escribir \n", tid);
  rwlock.wlock();
    printf("Thread %lu tiene el recurso para escribir \n", tid);
    // el thread hace uso del mutex:
    sleep(2); // hace sleep para que no pase tan rapido
    contador++; // hace uso del recurso compartido
    contadorLocal = contador;
    printf("Thread %lu termino de escribir. Valor del contador: %i \n", tid, contadorLocal);
  rwlock.wunlock();

  return args;
}

void* threadLectorDuracion2(void* args){
  auto tid = gettid();
  int contadorLocal;

  printf("Thread %lu esta esperando para leer \n", tid);
  rwlock.rlock();
    printf("Thread %lu tiene el recurso para leer \n", tid);
    // el thread hace uso del mutex:
    sleep(2); // hace sleep para que no pase tan rapido
    contadorLocal = contador; // hace uso del recurso compartido
    printf("Thread %lu termino de leer. Valor del contador: %i \n", tid, contadorLocal);
  rwlock.runlock();

  return args;
}

void* threadEscritorDuracion0(void* args){
  auto tid = gettid();
  int contadorLocal;

  printf("Thread %lu esta esperando para escribir \n", tid);
  rwlock.wlock();
    printf("Thread %lu tiene el recurso para escribir \n", tid);
    // el thread hace uso del mutex:

    contador++; // hace uso del recurso compartido
    contadorLocal = contador;
    printf("Thread %lu termino de escribir. Valor del contador: %i \n", tid, contadorLocal);
  rwlock.wunlock();

  return args;
}

void* threadLectorDuracion0(void* args){
  auto tid = gettid();
  int contadorLocal;

  printf("Thread %lu esta esperando para leer \n", tid);
  rwlock.rlock();
    printf("Thread %lu tiene el recurso para leer \n", tid);
    // el thread hace uso del mutex:
    contadorLocal = contador; // hace uso del recurso compartido
    printf("Thread %lu termino de leer. Valor del contador: %i \n", tid, contadorLocal);
  rwlock.runlock();

  return args;
}

void testDosLectoresUnEscritor(){
  // Caso dos lectores y un escritor a la vez
  contador = 0;
  int n = 10;

  printf("Este test creará 2 threads de lectura y uno de escritura a la vez. \n");
  sleep(1);

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
  free(threads);

  pthread_exit(NULL); // necesario para que no termine antes que sus threads hijos
}

void testDosEscritoresUnLector(){
  // Caso dos lectores y un escritor a la vez
  contador = 0;
  int n = 10;

  printf("Este test creará 2 threads de escritura y uno de lectura a la vez. \n");
  sleep(1);

  pthread_t *threads;
  threads = (pthread_t*)calloc(n*3, sizeof(pthread_t));
  for(int i = 0; i < n*3; i=i+3){
    pthread_t* p1 = &threads[i];
    pthread_create(&threads[i], NULL, threadEscritor, p1);
    pthread_t* p2 = &threads[i+1];
    pthread_create(&threads[i+1], NULL, threadEscritor, p2);
    pthread_t* p3 = &threads[i+2];
    pthread_create(&threads[i+2], NULL, threadLector, p3);
  }
  free(threads);

  pthread_exit(NULL); // necesario para que no termine antes que sus threads hijos
}

void testInanicionLectura(){
  contador = 0;
  int n = 20;

  printf("5 escritores, luego 1 lector y después 14 escritores \n");
  sleep(1);

  pthread_t *threads;
  threads = (pthread_t*)calloc(n, sizeof(pthread_t));
  
  for(int i = 0; i < 5; ++i){
    pthread_t* pi = &threads[i];
    pthread_create(&threads[i], NULL, threadEscritor, pi);
  }
  pthread_t* p2 = &threads[1];
  pthread_create(&threads[1], NULL, threadLector, p2);
  for(int i = 5; i < 20; ++i){
    pthread_t* pi = &threads[i];
    pthread_create(&threads[i], NULL, threadEscritor, pi);
  }
  free(threads);
  pthread_exit(NULL); // necesario para que no termine antes que sus threads hijos
}

void testInanicionEscritura(){
  contador = 0;
  int n = 20;

  printf("5 lectores, luego 1 escritor y después 14 lectores \n");
  sleep(1);

  pthread_t *threads;
  threads = (pthread_t*)calloc(n, sizeof(pthread_t));
  
  for(int i = 0; i < 5; ++i){
    pthread_t* pi = &threads[i];
    pthread_create(&threads[i], NULL, threadLector, pi);
  }
  pthread_t* p2 = &threads[1];
  pthread_create(&threads[1], NULL, threadEscritor, p2);
  for(int i = 5; i < 20; ++i){
    pthread_t* pi = &threads[i];
    pthread_create(&threads[i], NULL, threadLector, pi);
  }
  free(threads);
  pthread_exit(NULL); // necesario para que no termine antes que sus threads hijos
}

void veinteLectores(){
  contador = 0;
  int n = 20;
  printf("20 lectores\n");
  sleep(1);

  pthread_t *threads;
  threads = (pthread_t*)calloc(n, sizeof(pthread_t));
  
  for(int i = 0; i < n; ++i){
    pthread_t* pi = &threads[i];
    pthread_create(&threads[i], NULL, threadLector, pi);
  }
  free(threads);
  pthread_exit(NULL); // necesario para que no termine antes que sus threads hijos
}

int randomint(int min, int max){
  return min + (rand() % (int)(max - min));
}

void veinteAleatorios(){
  contador = 0;
  int n = 20;
  printf("20 aleatorios\n");
  sleep(1);

  pthread_t *threads;
  threads = (pthread_t*)calloc(n, sizeof(pthread_t));
  
  srand(314);

  int cantEscritores = 0;
  for(int i = 0; i < n; ++i){
    pthread_t* pi = &threads[i];
    if (rand()%2){
      pthread_create(&threads[i], NULL, threadLector, pi);
    }else{
      pthread_create(&threads[i], NULL, threadEscritor, pi);
      cantEscritores++;
    }
  }
  printf("La cantidad de lectores fue de: %i, y de escritores: %i\n", (n-cantEscritores), cantEscritores);
  free(threads);
  pthread_exit(NULL); // necesario para que no termine antes que sus threads hijos
}

void veinteAleatoriosDuracionAleatoria(){
  contador = 0;
  int n = 20;
  printf("20 aleatorios con duración aleatoria\n");
  sleep(1);

  pthread_t *threads;
  threads = (pthread_t*)calloc(n, sizeof(pthread_t));
  
  srand(314);

  int cantEscritores = 0;
  for(int i = 0; i < n; ++i){
    pthread_t* pi = &threads[i];
    if (rand()%2){
      int duracion = rand();
      if (duracion%3 == 0){
        pthread_create(&threads[i], NULL, threadLectorDuracion0, pi);
      }else if (duracion%3 == 1){
        pthread_create(&threads[i], NULL, threadLector, pi);
      }else{
        pthread_create(&threads[i], NULL, threadLectorDuracion2, pi);
      }
    }else{
      int duracion = rand();
      if (duracion%3 == 0){
        pthread_create(&threads[i], NULL, threadEscritorDuracion0, pi);
      }else if (duracion%3 == 1){
        pthread_create(&threads[i], NULL, threadEscritor, pi);
      }else{
        pthread_create(&threads[i], NULL, threadEscritorDuracion2, pi);
      }
      cantEscritores++;
    }
  }
  printf("La cantidad de lectores fue de: %i, y de escritores: %i\n", (n-cantEscritores), cantEscritores);
  free(threads);
  pthread_exit(NULL); // necesario para que no termine antes que sus threads hijos
}

void veinteEscritores(){
  contador = 0;
  int n = 20;
  printf("20 escritores\n");
  sleep(1);

  pthread_t *threads;
  threads = (pthread_t*)calloc(n, sizeof(pthread_t));
  
  for(int i = 0; i < n; ++i){
    pthread_t* pi = &threads[i];
    pthread_create(&threads[i], NULL, threadEscritor, pi);
  }
  free(threads);
  pthread_exit(NULL); // necesario para que no termine antes que sus threads hijos
}

bool continuar(){
  string ingresado;
  printf("¿Desea hacer otra consulta? (S/n)\n");
  cin >> ingresado;
  return (ingresado != "n");
}

int main(int argc, char const *argv[]) {
  string ingresado;
  menu();
  cin >> ingresado;
  if(ingresado == "Q" || ingresado == "q"){
    //nada, termina
  }else if (ingresado == "H" || ingresado == "h"){
    menu();
  }else if (ingresado == "1"){
    veinteLectores();
    
  }else if (ingresado == "2"){
    veinteEscritores();
    
  }else if (ingresado == "3"){
    testDosLectoresUnEscritor();

  }else if (ingresado == "4"){
  testDosEscritoresUnLector();    

  }else if (ingresado == "5"){
    testInanicionLectura();

  }else if (ingresado == "6"){
    testInanicionEscritura();

  }else if (ingresado == "7"){
    veinteAleatorios();

  }else if (ingresado == "8"){
    veinteAleatoriosDuracionAleatoria();

  }else{
    printf("Disculpas, no entendí ese comando. Intenta nuevamente (Q para salir, H para mostrar menú) \n");
  }
  return 0;
}
