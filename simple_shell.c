#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define PROMPT "#cisfun$ "
#define BUFFER_SIZE 1024

/* Function prototypes */
void execute_command(char *line);
char **parse_command(char *line);
char *find_command_path(char *command);
int execute_pipe(char *command1, char *command2);

extern char **environ;

/**
 * main - Entry point of the shell program.
 *
 * Return: Always 0 (Success)
 */
int main(void)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t nread;

	while (1)
	{
		/* Display prompt */
		write(STDOUT_FILENO, PROMPT, strlen(PROMPT));

		/* Read command line input */
		nread = getline(&line, &len, stdin);

		/* Handle end-of-file (Ctrl+D) */
		if (nread == -1)
		{
			write(STDOUT_FILENO, "\n", 1);
			break;
		}

		/* Remove newline character */
		if (line[nread - 1] == '\n')
			line[nread - 1] = '\0';

		/* Ignore empty lines */
		if (line[0] == '\0')
			continue;

		/* Check for "exit" command to terminate the shell */
		if (strcmp(line, "exit") == 0)
		{
			free(line);
			break;
		}

		/* Execute the command */
		execute_command(line);
	}

	free(line);
	return (0);
}

/**
 * execute_command - Executes a given command.
 * @command: The command to execute.
 */
void execute_command(char *command)
{
	char **argv;
	char *pipe_pos;
	pid_t pid;
	int status;
	char *path;

	/* Check for pipe */
	pipe_pos = strchr(command, '|');
	if (pipe_pos != NULL)
	{
		*pipe_pos = '\0';
		execute_pipe(command, pipe_pos + 1);
		return;
	}

	/* Check for echo */
	if (strncmp(command, "echo ", 5) == 0)
	{
		printf("%s\n", command + 5);
		return;
	}

	/* Parse the command into arguments */
	argv = parse_command(command);
	if (argv == NULL || argv[0] == NULL)
	{
		free(argv);
		return;
	}

	/* Check if the command is a file and executable */
	if (access(argv[0], X_OK) == 0)
	{
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			free(argv);
			exit(EXIT_FAILURE);
		}

		if (pid == 0)
		{
			/* Child process: execute the file directly */
			if (execve(argv[0], argv, environ) == -1)
			{
				perror("execve");
				free(argv);
				exit(EXIT_FAILURE);
			}
		}
		else
		{
			/* Parent process: wait for child to finish */
			wait(&status);
		}

		free(argv);
		return;
	}

	/* Find the command path */
	path = find_command_path(argv[0]);

	if (path == NULL || access(path, X_OK) == -1)
	{
		fprintf(stderr, "./shell: %s: command not found\n", argv[0]);
		free(argv);
		free(path);
		return;
	}

	/* Create a new process */
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		free(argv);
		free(path);
		exit(EXIT_FAILURE);
	}

	if (pid == 0)
	{
		/* Child process: execute the command */
		if (execve(path, argv, environ) == -1)
		{
			perror("execve");
			free(argv);
			free(path);
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		/* Parent process: wait for child to finish */
		wait(&status);
	}

	free(argv);
	free(path);
}

/**
 * execute_pipe - Executes two commands connected by a pipe.
 * @command1: The first command.
 * @command2: The second command.
 *
 * Return: 0 on success, -1 on failure.
 */
int execute_pipe(char *command1, char *command2)
{
	int pipefd[2];
	pid_t pid1, pid2;

	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		return (-1);
	}

	pid1 = fork();
	if (pid1 == -1)
	{
		perror("fork");
		return (-1);
	}

	if (pid1 == 0)
	{
		/* First child: write to pipe */
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
		execute_command(command1);
		exit(EXIT_SUCCESS);
	}

	pid2 = fork();
	if (pid2 == -1)
	{
		perror("fork");
		return (-1);
	}

	if (pid2 == 0)
	{
		/* Second child: read from pipe */
		close(pipefd[1]);
		dup2(pipefd[0], STDIN_FILENO);
		close(pipefd[0]);
		execute_command(command2);
		exit(EXIT_SUCCESS);
	}

	/* Parent process: close pipe and wait */
	close(pipefd[0]);
	close(pipefd[1]);
	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);

	return (0);
}

/**
 * parse_command - Splits a command line into arguments.
 * @line: The command line to split.
 *
 * Return: An array of arguments.
 */
char **parse_command(char *line)
{
	char **argv = NULL;
	char *token;
	size_t argc = 0;

	argv = malloc(sizeof(char *) * BUFFER_SIZE);
	if (argv == NULL)
	{
		perror("malloc");
		return (NULL);
	}

	token = strtok(line, " ");
	while (token != NULL)
	{
		argv[argc] = strdup(token);
		if (argv[argc] == NULL)
		{
			perror("strdup");
			free(argv);
			return (NULL);
		}
		argc++;
		token = strtok(NULL, " ");
	}

	argv[argc] = NULL;
	return (argv);
}

/**
 * find_command_path - Finds the full path of a command.
 * @command: The command to locate.
 *
 * Return: The full path of the command or NULL if not found.
 */
char *find_command_path(char *command)
{
	char *path = getenv("PATH");
	char *path_copy, *dir, *full_path;
	size_t command_len, dir_len;

	if (path == NULL)
		return (NULL);

	path_copy = strdup(path);
	if (path_copy == NULL)
	{
		perror("strdup");
		return (NULL);
	}

	command_len = strlen(command);

	dir = strtok(path_copy, ":");
	while (dir != NULL)
	{
		dir_len = strlen(dir);
		full_path = malloc(dir_len + command_len + 2);
		if (full_path == NULL)
		{
			perror("malloc");
			free(path_copy);
			return (NULL);
		}

		sprintf(full_path, "%s/%s", dir, command);

		if (access(full_path, X_OK) == 0)
		{
			free(path_copy);
			return (full_path);
		}

		free(full_path);
		dir = strtok(NULL, ":");
	}

	free(path_copy);
	return (NULL);
}
