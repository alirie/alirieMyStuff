#include <stdio.h>

void exchange(int * x, int * y)
{
	int help = *x;
	*x = *y;
	*y = help;
}

int main(void)
{
	int x = 123;
	int y = 345;

	printf("x: %i y: %i\n", x, y);
	exchange(&x, &y);
	printf("x: %i y: %i\n", x, y);
	
	return 0;
}