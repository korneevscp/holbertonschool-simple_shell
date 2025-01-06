#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define PROMPT "#cisfun$ "
#define BUFFER_SIZE 1024

/* Function prototypes */
void execute_command(char *command);

int main(void)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t nread;
	while (1)
    {
    /* Display the prompt */
    write(STDOUT_FILENO, PROMPT, strlen(PROMPT));
	/* Read a line from the user */
	nread = getline(&line, &len, stdin);
	/* Handle EOF (Ctrl+D) */
	if (nread == -1)
	{
	write(STDOUT_FILENO, "\n", 1);
	break;
	}
	/* Remove the newline character */
	if (line[nread - 1] == '\n')
	line[nread - 1] = '\0';
        /* Ignore empty commands */
        if (line[0] == '\0')
	continue;
	/* Execute the command */
	execute_command(line);
	}
free(line);
return 0;
}
void execute_command(char *command)
{
	pid_t pid;
	int status;
	char *argv[2];
	argv[0] = command; /* Command to execute */
	argv[1] = NULL;    /* Null-terminated argument list */
	pid = fork();
	if (pid == -1)
		{
		perror("fork");
		exit(EXIT_FAILURE);
		}
	if (pid == 0)
		{
		/* Child process */
		if (execve(command, argv, environ) == -1)
		{
		perror("./shell");
		exit(EXIT_FAILURE);
		}
	}
    else
	{
	//* Parent process */
	wait(&status);
	}
}