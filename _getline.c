#include "main.h"
/**
 * _getline - function to implement getline
 * Return: NULL
 */

char *_getline(void)
{
	char *line = NULL;
	size_t size = 0;
	int checker;

	checker = getline(&line, &size, stdin);
	if (checker == -1)
		free(line);
	else
		return (line);
	return (NULL);
}
