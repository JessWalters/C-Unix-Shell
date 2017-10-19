#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define MAX_LINE 80 /* The maximum length command */

void runCommand(char *args[], int async) {
	int status;
	pid_t pid = fork();

	if (pid == 0) {
		if (execvp(args[0], args) < 0)
			printf("Error in execvp");
	}
	else {
		if (async == 0)
			while(wait(&status) != pid);
	}
}

int isAsync(char *args[]) {
	int i = 0;
	while (args[i]) {
		if (strcmp(args[i], "&") == 0){
			return 1;
		}
		i++;
	}
	return 0;
}

int main(void) {
	char *args[MAX_LINE/2 + 1]; /* command line arguments */
	int should_run = 1; /* flag to determine when to exit program */
	int status;
	char *cmd = malloc(sizeof(char) * MAX_LINE);
	while (should_run) {
		printf("osh>");
		fflush(stdout);
		fgets(cmd, MAX_LINE, stdin);

		char *tmp = strtok(cmd, " ");
		int i = 0;
		while(tmp != NULL) {
			args[i++] = strcat(tmp, "\0");
			tmp = strtok(NULL, " ");
		}
		args[i -1][strlen(args[i-1]) - 1] = 0;
		for (i = i; i< (MAX_LINE/2 + 1); i++)
			args[i] = NULL;

		if (strcmp(args[0], "exit") == 0){
			should_run = 0;
		}
		int async = isAsync(args);
		runCommand(args, async);
		/**
		 * After reading user input, the steps are:
		 * (1) fork a child process using fork()
		 * (2) the child process will invoke execvp()
		 * (3) if command included &, parent will invoke wait()
		 */
	}
	return 0;
}
