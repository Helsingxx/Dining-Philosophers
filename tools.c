#include "philosophers.h"

int	skip_int(char *arg)
{
	int	a;

	a = 0;
	while (arg[a] == ' ')
		a++;
	if (arg[a] == '-')
		a++;
	while (arg[a] && arg[a] != ' ')
		a++;
	while (arg[a] == ' ')
		a++;
	return (a);
}

int	ft_strlen(const char *s)
{
	int	a;

	a = 0;
	if (!s)
		return (0);
	while (s[a] != '\x00')
		a++;
	return (a);
}

void chad_wastetime(int milliseconds)
{
	struct timeval timenow;
	struct timeval timefirst;
	int x;
	int c;

	c = 1;
	x = 0;
	gettimeofday(&timefirst, NULL);
	gettimeofday(&timenow, NULL);
	while (((timenow.tv_sec * 1000 + timenow.tv_usec / 1000)
			- (timefirst.tv_sec * 1000 + timefirst.tv_usec / 1000)) < milliseconds)
	{
		usleep(10);
		x++;
		gettimeofday(&timenow, NULL);
	}
	//gettimeofday(&timenow, NULL);
//	printf("wasted %ld\n", ((timenow.tv_sec * 1000 + timenow.tv_usec / 1000)
	//		- (timefirst.tv_sec * 1000 + timefirst.tv_usec / 1000)));
}