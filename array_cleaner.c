#include "main.h"
/*
 * array_cleaner - function to free array
 * @array: array needed to be freed
 */
void array_cleaner(char **array)
{
	int i = 0;

	while (array[i])
	{
		free(array[i]);
		i++;
	}	
	free(array);
}
