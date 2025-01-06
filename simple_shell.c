#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_CMD_LENGTH 1024

int main(void)
{
    char command[MAX_CMD_LENGTH];
    char *argv[2];
    pid_t pid;
    int status;

    while (1)
    {
        // Display prompt
        printf("#cisfun$ ");
        fflush(stdout);

        // Read command from user
        if (fgets(command, MAX_CMD_LENGTH, stdin) == NULL)
        {
            // Handle EOF (Ctrl+D)
            printf("\n");
            break;
        }

        // Remove the newline character
        command[strcspn(command, "\n")] = '\0';

        // Check if the command is empty
        if (strlen(command) == 0)
            continue;

        // Prepare arguments
        argv[0] = command;
        argv[1] = NULL;

        // Fork a child process
        pid = fork();
        if (pid == -1)
        {
            perror("fork");
            continue;
        }

        if (pid == 0)
        {
            // Child process: Execute the command
            if (execve(command, argv, environ) == -1)
            {
                perror("./shell");
                exit(EXIT_FAILURE);
            }
        }
        else
	{
	// Parent process: Wait for the child to finish
	wait(&status);
		}
	}

	return 0;
}

