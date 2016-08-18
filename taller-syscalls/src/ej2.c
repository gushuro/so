#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/reg.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int main(int argc, char* argv[]) {
	int status;
	pid_t child;
	if (argc <= 1) {
		fprintf(stderr, "Uso: %s commando [argumentos ...]\n", argv[0]);
		exit(1);
	}

	/* Fork en dos procesos */
	child = fork();
	if (child == -1) { perror("ERROR fork"); return 1; }
	if (child == 0) {
		/* Solo se ejecuta en el hijo */
		ptrace(PTRACE_TRACEME, NULL, NULL, NULL);
		execvp(argv[1], argv+1);
		/* Si vuelve de exec() hubo un error */
		perror("ERROR child exec(...)"); exit(1);
	} else {
		/* Solo se ejecuta en el padre */
		while(1) {
			if (wait(&status) < 0) { perror("waitpid"); break; }
			int sysno = ptrace(PTRACE_PEEKUSER, child, 4*ORIG_EAX, NULL);
			if (sysno == 62){
				puts("¡Se ha hecho Justicia!");
				kill(child, SIGTERM);
				sleep(1);
				kill(child, SIGKILL);
				break;	
			} else {
				ptrace(PTRACE_SYSCALL, child, NULL, NULL);
			}
			if (WIFEXITED(status)) {
				break; /* Proceso terminado */
			}
		}
	}
	return 0;
}
