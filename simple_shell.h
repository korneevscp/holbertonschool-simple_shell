#ifndef SIMPLE_SHELL_H
#define SIMPLE_SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

/* Macro for the shell prompt */
#define PROMPT "#cisfun$ "

/* Global environment variable */
extern char **environ;

/* Function prototypes */

/**
 * find_command_path - Finds the full path of a command.
 * @command: The command to locate.
 *
 * Return: The full path of the command or NULL if not found.
 */
char *find_command_path(char *command);

/**
 * execute_command - Executes a given command.
 * @command: The command to execute.
 */
void execute_command(char *command);

#endif /* SIMPLE_SHELL_H */