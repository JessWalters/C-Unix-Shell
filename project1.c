#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAX_LINE 80 /* The maximum length command */

void parse(char *cmd, char** args) {
	while(*cmd != '\0') {
		while(*cmd == ' ') {
			*cmd++;
		}
		*args++ = cmd;
		while(*cmd != '\0' && *cmd != ' ') {
			cmd++;
		}
	}
	*args++ = '\0';
}

void runCommand(char **args) {
	int status;
	pid_t pid = fork();

	if (pid == 0) {
		execvp(*args, args);
	}
	else {
		while(wait(&status) != pid);
	}
}

int main(void) {
	char *args[MAX_LINE/2 + 1]; /* command line arguments */
	int should_run = 1; /* flag to determine when to exit program */
	int status;
	char *cmd;
	while (should_run) {
		printf("osh>");
		fflush(stdout);
		scanf("%s", cmd);	

		parse(cmd, args);
		if (strcmp(args[0], "exit") == 0){
			exit(0);
		}
		runCommand(args);
		/**
		 * After reading user input, the steps are:
		 * (1) fork a child process using fork()
		 * (2) the child process will invoke execvp()
		 * (3) if command included &, parent will invoke wait()
		 */
	}
	return 0;
}
