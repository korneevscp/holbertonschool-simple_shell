#include "main.h"

/**
 * splitter - splits given string into parts
 *
 * @str: given string
 *
 * Return: modified char array.
 */

char **splitter(char *str, char *delim)
{
	char *token;
	char **array;
	int i = 0;

	if (str == NULL || str[0] == '\0')
		return (NULL);
	array = malloc(strlen(str) * sizeof(char *));
	token = strtok(str, delim);
	while (token != NULL)
	{
		*(array + i) = strdup(token);
		if (!*(array + i))
		{
			free(*(array + i));
			exit(99);
		}
		i++;
		token = strtok(NULL, delim);
	}
	*(array + i) = NULL;
	return (array);
}
