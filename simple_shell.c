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

    if (argv == NULL)
        return; 
    pid = fork();
    if (pid == -1)
    {
    perror("fork");
    free(argv);
    exit(EXIT_FAILURE);
    }
    if (pid == 0)
    {
        /* Child process */
        if (execve(argv[0], argv, environ) == -1)
        {
            perror("./shell");
            free(argv);
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        /* Parent process */
        wait(&status);
    }
    free(argv);
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
