#include <stdio.h>
#include <unistd.h>


int main(void)
{
	int my_ppid;

	my_ppid = getppid();
	printf("%u\n", my_ppid);

	return (0);
}
