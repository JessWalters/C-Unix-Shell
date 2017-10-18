#include <stdio.h>
#include <unistd.h>

#define MAX_LINE 80 /* The maximum length command */

int main(void) {
	char *args[MAX_LINE/2 + 1]; /* command line arguments */
	int should_run = 1; /* flag to determine when to exit program */
	int status;
	while (should_run) {
		printf("osh>");
		fflush(stdout);
		scanf("%s", cmd);
		printf("\n %s", &cmd);

		int pid = fork();

		if (pid == 0) {
			execvp(cmd, args);
			exit(1);
		}
		else {
			waitpid(pid, &status, 0);
		}
		/**
		 * After reading user input, the steps are:
		 * (1) fork a child process using fork()
		 * (2) the child process will invoke execvp()
		 * (3) if command included &, parent will invoke wait()
		 */
	}
	return 0;
}
