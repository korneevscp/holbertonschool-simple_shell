#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "simple_shell.h"

#define PROMPT "#cisfun$ "
#define BUFFER_SIZE 1024

/* Function prototypes */
void execute_command(char *line);
char **parse_command(char *line);
char *find_command_path(char *command);

extern char **environ;

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

void execute_command(char *line)
{
    pid_t pid;
    int status;
    char **argv = parse_command(line);
    char *command_path;

    if (argv == NULL)
        return;

    /* Handle built-in "exit" command */
    if (strcmp(argv[0], "exit") == 0)
    {
        free(argv);
        free(line);
        exit(0);
    }

    /* Handle built-in "env" command */
    if (strcmp(argv[0], "env") == 0)
    {
        for (char **env = environ; *env != NULL; env++)
        {
            write(STDOUT_FILENO, *env, strlen(*env));
            write(STDOUT_FILENO, "\n", 1);
        }
        free(argv);
        return;
    }

    /* Find the full path of the command */
    command_path = find_command_path(argv[0]);

    if (command_path == NULL)
    {
        fprintf(stderr, "./shell: %s: command not found\n", argv[0]);
        free(argv);
        return;
    }

    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        free(argv);
        free(command_path);
        exit(EXIT_FAILURE);
    }

    if (pid == 0)
    {
        /* Child process */
        if (execve(command_path, argv, environ) == -1)
        {
            perror("./shell");
            free(argv);
            free(command_path);
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        /* Parent process */
        wait(&status);
    }

    free(argv);
    free(command_path);
}

char **parse_command(char *line)
{
    char **argv = NULL;
    char *token;
    size_t argc = 0;
    size_t capacity = 10;

    argv = malloc(sizeof(char *) * capacity);
    if (argv == NULL)
    {
        perror("malloc");
        return NULL;
    }

    token = strtok(line, " ");
    while (token != NULL)
    {
        if (argc >= capacity)
        {
            capacity *= 2;
            argv = realloc(argv, sizeof(char *) * capacity);
            if (argv == NULL)
            {
                perror("realloc");
                return NULL;
            }
        }

        argv[argc++] = token;
        token = strtok(NULL, " ");
    }

    argv[argc] = NULL;
    return argv;
}

char *find_command_path(char *command)
{
    char *path = getenv("PATH");
    char *path_copy, *dir, *full_path;
    size_t command_len, dir_len;

    if (path == NULL)
        return NULL;

    path_copy = strdup(path);
    if (path_copy == NULL)
    {
        perror("strdup");
        return NULL;
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
            return NULL;
        }

        sprintf(full_path, "%s/%s", dir, command);

        if (access(full_path, X_OK) == 0)
        {
            free(path_copy);
            return full_path;
        }

        free(full_path);
        dir = strtok(NULL, ":");
    }

    free(path_copy);
    return NULL;
}
