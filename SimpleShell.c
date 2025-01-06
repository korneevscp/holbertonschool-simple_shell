#include "simple_shell.h"

/**
 * create_shell - Allocate memory for the simple shell and initialize its values.
 * @shell:  Double pointer to the simple shell.
 * @envp: Environmental variables passed directly from the main function
 *
 * Return: Indicates whether the shell was successfully created or not.
 */
void create_shell(SimpleShell_t **shell, char **envp)
{
*shell = malloc(sizeof(SimpleShell_t));
if (shell == NULL)
return;

((*shell)->line_num) = 0;
((*shell)->exit_status) = 0;
((*shell)->is_active) = TRUE;
(*shell)->enviornment = envp;
(*shell)->path_variable = split_string(getenv("PATH"), ":");
(*shell)->os_command_path = NULL;
(*shell)->builtin = NULL;
}

/**
 * parse_line - Analyze a single line from a script or the REPL.
 * @shell: Double pointer to the simple shell.
 * @new_line: The line to be parsed.

 *
 * Description: WILL REQUIRE SIGNIFICANT REWORK TO SUPPORT PIPES 
*/
void parse_line(SimpleShell_t *shell, char *new_line)
{
shell->command_args = split_string(new_line, " ");

shell->builtin = get_builtin(shell->command_args[0]);

shell->os_command_path = find_command_path(shell);

if (shell->builtin != NULL)
shell->builtin(shell);
else if (shell->os_command_path != NULL)
create_new_process(shell);
else
throw_error(shell, 2);

free_array(shell->command_args);
free(shell->os_command_path);
}
/**
 * free_shell - Releases memory allocated for the interpreter and its properties
 * @shell: Double pointer to the simple shell.
 */
void free_shell(SimpleShell_t *shell)
{
free_array(shell->path_variable);

free(shell);
}

/**
 * free_array - Frees memory allocated by the split_string function.
 * @an_array: The array to be deallocated.
*/
void free_array(char **an_array)
{
int i;

for (i = 0; an_array[i]; i++)
free(an_array[i]);

free(an_array);
}

/**
 * get_builtin - Searches for and identifies built-in commands.
 * @command:  The command to search for, provided as a string.
 *
 * Return: Function pointer to the command to execute.
*/
void (*get_builtin(char *command))(SimpleShell_t *)
{
int i;
BuiltInCommand_t builtins[] = {
{"exit", quit_repl},
{"quit", quit_repl},
{"env", print_env_variables},
{NULL, NULL}
};

for (i = 0; builtins[i].name; i++)
{
if (strcmp(command, builtins[i].name) == 0)
return (builtins[i].exec);
}
return (NULL);
}
