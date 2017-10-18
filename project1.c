#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define MAX_LINE 80 /* The maximum length command */

void parse(char *cmd, char** args) {
	while(*cmd != '\0' && *cmd != '\n') {
		while(*cmd == ' ') {
			*cmd++;
		}
		*args++ = cmd;
		while(*cmd != '\0' && *cmd != ' ' && *cmd != '\n') {
			cmd++;
		}
	}
	*args++ = '\0';
}

void runCommand(char **args, int async) {
	int status;
	pid_t pid = fork();

	if (pid == 0) {
		execvp(*args, args);
	}
	else {
		if (async == 0)
			while(wait(&status) != pid);
	}
}

int isAsync(char *args[]) {
	int i = 0;
	printf("here");
	fflush(stdout);
	while (strcmp(args[i], "\n") != 0 && strcmp(args[i], "\0") != 0) {
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
	char *cmd;
	while (should_run) {
		printf("osh>");
		fflush(stdout);
		fgets(cmd, MAX_LINE, stdin);	

		parse(cmd, args);
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
