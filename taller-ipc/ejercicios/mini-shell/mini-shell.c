#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>  /* pid_t */
#include <sys/wait.h>   /* waitpid */
#include <unistd.h>     /* exit, fork */

void pexit(char *error) {
	perror(error);
	exit(EXIT_FAILURE);
}


int run(char *program_name[], char **program_argv[], unsigned int count) {
	/* TODO: Implemenar */

	int pipes[count][2];
	for (int i = 0; i < count; ++i) {

		if (pipe(pipes[i])<0){
			pexit("pipe error");
		}
	}
	for (int i = 0; i < count; ++i){
		pid_t pid = fork();
		if (pid == 0) {
			// Esto se ejecuta en los hijos.
			// cierro todos los pipes que no me corresponden
			for (int j = 0; j < count; ++j)
			{
				if (j != i+1) {
					close(pipes[j][1]);
				}
				if (j != i) {
					close(pipes[j][0]);
				}
			}
			if (i == 0) {
				close(pipes[i][0]);
				dup2(pipes[1][1],1);
			} else if (i == count-1) {
				dup2(pipes[i][0], 0);
			} else {
				dup2(pipes[i+1][1],1);
				dup2(pipes[i][0], 0);
			}
			//sleep(1);
			execvp(program_name[i], program_argv[i]);
			break;
		} else{
			close(pipes[i][0]);
			close(pipes[i][1]);
		}

	}
			sleep(1);





	return 0;
}

int main(int argc, char* argv[]) {
	/* Parsing de "ls -al | wc | awk '{ print $2 }'" */
	char *program_name[] = {
		"/bin/ls",
		"/usr/bin/wc",
		"/usr/bin/awk",
	};

	char *ls_argv[] = {"ls", "-al", NULL};
	char *wc_argv[] = {"wc", NULL};
	char *awk_argv[] = {"awk", "{ print $2 }", NULL};

	char **program_argv[] = {
		ls_argv,
		wc_argv,
		awk_argv,
	};

	unsigned int count = 3;

	int status = run(program_name, program_argv, count);

	printf("[+] Status : %d\n", status);

	return 0;
}
