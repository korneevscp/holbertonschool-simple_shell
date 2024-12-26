#include "main.h"

/**
 * main - main function of project
 *
 * Return: always 0
 */

int main(void)
{
	char *my_prompt, **array, *temp, *command, **path, *env_var;
	int status, i = 0;
	pid_t pid;

	path = pathfinder();
	while (1)
	{
		if (isatty(STDIN_FILENO))
			printf("#cisfun$ ");
		my_prompt = _getline();
		if (!my_prompt)
		{
			break;
		}
		array = splitter(my_prompt, " \n\t");
		free(my_prompt);
		if (!*array)
		{
			free(array);
			continue;
		}
		 if (strcmp(array[0], "exit") == 0)
                {
                        if (array[1] != NULL)
                        {
                                exit_shell(2);
                        }
                        else
                        {
                                exit_shell(0);
                        }
                }
		else
                        if (strcmp(array[0], "env") == 0)
                        {
                                if (array[1] && strcmp(array[1], "-i") == 0)
                                {
                                                env_var = array[2];
                                                env_shell(env_var);
                                }
                                else
                                        env_shell(NULL);
                        }
                else
		pid = fork();
		if (pid == 0)
		{
			command = strdup(array[0]);
			execve(array[0], array, environ);
			while (path[i])
			{
				execve(array[0], array, environ);
				temp = malloc(strlen(path[i]) + strlen(command) + 1);
				strcpy(temp, path[i]);
				strcat(temp, command);
				free(array[0]);
				array[0] = strdup(temp);
				free(temp);
				i++;
			}
			printf("No such file or directory\n");
			exit(EXIT_FAILURE);
		}
		else
			wait(&status);
		for (i = 0; *(array + i); i++)
			free(*(array + i));
		free(array);
	}
	for (i = 0; *(path + i); i++)
		free(*(path + i));
	free(path);
	free(my_prompt);
	return (0);
}
/*
 * exit_shell - function to implement exit command
 * @status: exit status
 */

void exit_shell(int status)
{
        exit(status);
}

/*
 * env_shell - function to implement env command
 * @env_var: pointer to an array of env variables
 */
void env_shell(char *env_var)
{
        char *env_vars[2], **env;
        if(env_var)
        {
                env_vars[0] = env_var;
                env_vars[1] = NULL;
                execve("/usr/bin/env", env_vars, NULL);
        }
        else
        {
                for (env = environ; *env != NULL; env++)
                {
                        printf("%s\n", *env);
                }
        }
}
