#include <stdio.h>
#include <string.h>
#include "main.h"
/**
 * pathfinder - function to locate file in the path
 * Return: array
 */

char **pathfinder(void)
{
	char **array = NULL;
	char **environment = malloc(10000);
	char *temp = NULL;
	int i, j = 0, num = 0;
	
	while (environ[j])
	{
		environment[j] = malloc(strlen(environ[j]) + 1);
		strcpy(environment[j], environ[j]);
		j++;
		num++;
	}
	j = 0;
	while (environment[j])
	{
		array = splitter(environment[j], "=");
		if (!strcmp(*array, "PATH"))
		{
			if (!*(array + 1))
				break;
			temp = strdup(*(array + 1));
			for (i = 0; i < 2; i++)
			{
				free(*(array + i));
			}
			free(array);
			break;
		}
		for (i = 0; i < 2; i++)
		{
			free(*(array + i));
		}
		free(array);
		j++;
	}
	array = splitter(temp, ":");
	if (!array)
		return (NULL);
	i = 0;
	while (array[i])
	{
		array[i] = realloc(array[i], strlen(array[i]) + 2);
		strcat(array[i], "/");
		i++;
	}
	if (temp)
		free(temp);
	for (i = 0; i < num; i++)
		free(environment[i]);
	free(environment);
	return (array);
}

