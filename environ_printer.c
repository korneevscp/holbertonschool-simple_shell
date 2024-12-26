#include "main.h"
/*
 * environ_printer - function that prints the environment.
 * Return: nothing
 */
void environ_printer(void)
{
	int i = 0;

	while (environ[i])
	{
		printf("%s\n", environ[i]);
		i++;
	}
}
