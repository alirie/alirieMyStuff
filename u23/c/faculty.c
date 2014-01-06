#include <stdio.h>

int main(void)
{
	int x;
	int y = 1;
	for(x = 2; x<=10; x++)
	{
		y*=x;
	}
	printf("Faculty from 0 to 10: %i \n", y);
	return 0;
}