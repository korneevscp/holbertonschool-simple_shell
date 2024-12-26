#ifndef SIMPLE_SHELL_H
#define SIMPLE_SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define TRUE 1
#define FALSE 0

#define PATH_MAX 4096

/**
 /**
 * struct _SimpleShell - Represents the current state of the shell.
 * @line_num: The current line number in the script.
 * @exit_status: EXIT_SUCCESS (0) or EXIT_FAILURE (non-zero).
 * @is_active: TRUE (1) or FALSE (0) — indicates if the REPL loop is still running.
 * @path_variable: The PATH variable inherited from the environment as a string array.
 * @environment: The environmental variables.
 * @builtin: Function pointer to a built-in command, if applicable.
 * @os_command_path: Path to the executable file to invoke, if any.
 * @command_args: Arguments parsed from the last input line.
 *
 * Description: Defines the parameters that make up the core interpreter.
 */
typedef struct _SimpleShell
{
	int line_num;
	int exit_status;
	int is_active;
	char **path_variable;
	char **enviornment;
	void (*builtin)(struct _SimpleShell *);
	char *os_command_path;
	char **command_args;
} SimpleShell_t;

/**
 * struct BuiltInCommand_s - Represents the structure for defining built-in commands.
 * @name: The name of the command.
 * @exec:Function pointer to the function that executes the command. 
 */
typedef struct BuiltInCommand_s
{
	char *name;
	void (*exec)(SimpleShell_t *self);
} BuiltInCommand_t;

void launch_repl(SimpleShell_t *shell);
void read_script(SimpleShell_t *shell);

void create_shell(SimpleShell_t **shell, char **envp);
void parse_line(SimpleShell_t *shell, char *new_line);
void free_shell(SimpleShell_t *shell);
void free_array(char **an_array);

char *take_input();
int is_only_spaces(char *string);
int num_of_spaces(char *string, char *delimiter);
char **split_string(char *string_to_split, char *delimiter);

void (*get_builtin(char *command))(SimpleShell_t *);
void clear_screen(SimpleShell_t *self);
void quit_repl(SimpleShell_t *self);
void print_env_variables(SimpleShell_t *self);
char *_getenv(char **envp, char *key);

char *find_command_path(SimpleShell_t *self);
char *create_test_path(char *dir_path, char *command);
void create_new_process(SimpleShell_t *shell);

void throw_error(SimpleShell_t *shell, int error_num);

#endif
