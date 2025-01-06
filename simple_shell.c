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

extern char **environ;

#define PROMPT "#cisfun$ "

/* Function to find the command path */
char *find_command_path(char *command);

/* Function to execute commands */
void execute_command(char *command);

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
    pid_t pid;
    int status;
    char *argv[2];
    char *path;

    /* Set up arguments for execve */
    argv[0] = command;
    argv[1] = NULL;

    /* Find the command path */
    path = find_command_path(command);

    if (path == NULL || access(path, X_OK) == -1)
    {
    fprintf(stderr, "./shell: %s: command not found\n", command);
    return;
    }

    /* Create a new process */
    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0)
    {
        /* Child process: execute the command */
        if (execve(path, argv, environ) == -1)
        {
            perror("execve");
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        /* Parent process: wait for child to finish */
        wait(&status);
    }
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
