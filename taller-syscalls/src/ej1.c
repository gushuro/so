#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/reg.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>


int finished;
int count;
void handler(int signal){
	puts("ya va! q onda?");
	count++;
}

void handlerpadre(int signal){
	finished = 1;
}
int main(int argc, char* argv[]) {
	int status;
	pid_t child;
	finished = 0;
	count = 0;
	if (argc <= 1) {
		fprintf(stderr, "Uso: %s commando [argumentos ...]\n", argv[0]);
		exit(1);
	}

	/* Fork en dos procesos */
	child = fork();
	if (child == -1) { perror("ERROR fork"); return 1; }
	if (child == 0) {
		/* Solo se ejecuta en el hijo */
		signal(SIGURG, handler);

		pid_t padre = getppid();
		while(count < 5){
		}
		kill(padre, SIGINT);



		execvp(argv[1], argv+1);
		/* Si vuelve de exec() hubo un error */
		perror("ERROR child exec(...)"); exit(1);
	} else {
		/* Solo se ejecuta en el padre */
		signal(SIGINT, handlerpadre);
		while(!finished){
			sleep(1);
			if (finished){break;}
			puts("sup migo!");
			if (finished){break;}
			kill(child, SIGURG);
		}
		while(1) {
			if (wait(&status) < 0) { perror("waitpid"); break; }
			if (WIFEXITED(status)) break; /* Proceso terminado */
		}
	}
	return 0;
}
