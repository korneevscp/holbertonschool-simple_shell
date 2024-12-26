#include "simple_shell.h"

/**
 * quit_repl - quit interactive mode
 * @shell: double pointer back to shell interpreter
 * Description: PLACEHOLDER
 */
void quit_repl(SimpleShell_t *shell)
{
	(shell->is_active) = FALSE;
}

/**
 * print_env_variables - print list of enviornmental variables.
 * @shell: double pointer back to shell interpreter
 * Description: PLACEHOLDER
 */
void print_env_variables(SimpleShell_t *shell)
{
	int i = 0;

	do {
		printf("%s\n", shell->enviornment[i++]);
	} while (shell->enviornment[i]);
}
