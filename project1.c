#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <ctype.h>

#define MAX_LINE 80 /* The maximum length command */

void runCommand(char *args[], int async) {
    // Fork and run command on child process
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
    // Check if & is in the cmd and mark it as async if it is
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
    char *history[10];
	int should_run = 1; /* flag to determine when to exit program */
	char *cmd = (char *) malloc(sizeof(char) * MAX_LINE);
    char *hist_tmp = (char *) malloc(sizeof(char) * MAX_LINE);
    char *tmp2 = (char *) malloc(sizeof(char) * MAX_LINE);

    // Initialize history
    for (int k = 0;  k < 10;  ++k)
        history [k] = malloc (sizeof(char) * MAX_LINE);

	while (should_run) {
		printf("osh>");
		fflush(stdout);
		fgets(cmd, MAX_LINE, stdin);

        if (strcmp(cmd, "exit\n") == 0){
            should_run = 0;
            // Free malloced vars
            for (int k = 0;  k < 10;  ++k)
                free(history [k]);
            free(tmp2);
            free(hist_tmp);
            free(cmd);

            continue;
        }
        // Displays history
        if (strcmp(cmd, "history\n") == 0) {
            // Count number of elements, this is only done for purpose of following assignment formatting
            // Count is useless otherwise
            int count = 0;
            for (int c = 0; c < 10; c++) {
                if (strcmp(history[c], "") != 0)
                    count++;
            }
            for (int b = 0; b < 10; b++) {

                if (strcmp(history[b], "") != 0) {
                    printf("%d %s", count, history[b]);
                    fflush(stdout);
                    count--;
                }
            }
            continue;
        }
        // Checks to run last command
        if (strcmp(cmd, "!!\n") == 0) {
            strcpy(cmd, history[0]);
        }
        // checks to run a N command from history
        if (cmd[0] == '!') {
            if (isdigit(cmd[1])){
                // Converts char to int
                int command_num = cmd[1] - '0';
                strcpy(cmd, history[command_num]);
            }

        }

        //Save to history
        for(int j = 0; j < 10; j++) {
            // First element is cmd
            if (j == 0) {
                strcpy(hist_tmp, history[j]);
                strcpy(history[j], cmd);
                continue;
            }
            // Last element is discarded
            if (j == 9) {
                strcpy(history[j], hist_tmp);
                break;
            }
            // For all others just shift up one spot
            strcpy(tmp2, hist_tmp);
            strcpy(hist_tmp, history[j]);
            strcpy(history[j], tmp2);
        }

        // Parse the cmd using tokens
		char *tmp = strtok(cmd, " ");
		int i = 0;
		while(tmp != NULL) {
			args[i++] = strcat(tmp, "\0");
			tmp = strtok(NULL, " ");
		}
		args[i -1][strlen(args[i-1]) - 1] = 0;

        // Filling in the rest of the indexes as Null
		for (i = i; i< (MAX_LINE/2 + 1); i++)
			args[i] = NULL;

        // check for async command
		int async = isAsync(args);

        // Run the given command
		runCommand(args, async);
	}
	return 0;
}
