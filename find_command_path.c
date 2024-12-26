#include "simple_shell.h"

/**
 * find_command_path - attempts to find executable path to argv[0]
 * @shell: double pointer back to interpreter.
 * Return: pointer if found, NULL not found
 */
char *find_command_path(SimpleShell_t *shell)
{
	struct stat file_info;
	char *command_path;
	char *test_path;
	int i = 0;

	command_path = malloc(sizeof(char) * 255);

	snprintf(command_path, 255, "%s", shell->command_args[0]);

	while (shell->path_variable[i])
	{
		if (stat(command_path, &file_info) == 0)
			return (command_path);

		test_path = create_test_path(
				shell->path_variable[i++],
				shell->command_args[0]
		);
		snprintf(command_path, 255, "%s", test_path);
		free(test_path);
	}
	free(command_path);

	return (NULL);
}

/**
 * create_test_path - Concatenate command argv[0] to each directory path
 * @dir_path: pointer a one dir PATH
 * @command: command
 *
 * Return: pointer string dir + cmd or NULL error
 */
char *create_test_path(char *dir_path, char *command)
{
	int dir_path_len, command_len, total_path_len;
	int i = 0, j = 0;
	char *test_path;

	dir_path_len = strlen(dir_path);
	command_len = strlen(command);
	total_path_len = dir_path_len + command_len;

	test_path = malloc(sizeof(char) * total_path_len + 2);

	while (dir_path[i])
	{
		test_path[i] = dir_path[i];
		i++;
	}
	test_path[i] = '/';
	i++;
	while (command[j])
		test_path[i++] = command[j++];

	test_path[i] = '\0';

	return (test_path);
}

/**
 * create_new_process - Forks a child process that is possessed by our argv
 * @shell: double-pointer back to the interpreter
 *
 * Return: void
 */
void create_new_process(SimpleShell_t *shell)
{
	pid_t id;
	int status;

	id = fork();
	if (id == -1)
		perror("Fork failed");
	else if (id > 0)
		wait(&status);
	else if (id == 0)
		execve(
			shell->os_command_path,
			shell->command_args,
			shell->enviornment
		);
	if ((WIFEXITED(status)))
		(shell->exit_status) = WEXITSTATUS(status);
	if (id != 0)
	{
		fflush(stdout);
		fflush(stdin);
	}
}
/**
 * throw_error - dispaly an error message by designated number
 * @shell: double-pointer back to the interpreter (includes error num)
 * @error_num: number to invoke corresponding error.
 */
void throw_error(SimpleShell_t *shell, int error_num)
{
	char specific_error[20];
	char error_message[255];

	if (error_num == 1)
		snprintf(specific_error, 20, "%s", "Permission denied\n");
	else if (error_num == 2)
		snprintf(specific_error, 20, "%s", "not found\n");
	else if (error_num == 3)
		snprintf(specific_error, 20, "%s", "Can't open\n");
	else
		snprintf(specific_error, 20, "%s", "Unknown Error\n");

	snprintf(
		error_message,
		255,
		"%s: %d: %s: %s",
		getenv("_"),
		shell->line_num,
		shell->command_args[0],
		specific_error
	);

	fprintf(stderr, "%s", error_message);
}
