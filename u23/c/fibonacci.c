#include <stdio.h>

int main(void)
{
	int c;
	int a = 0;
	int b = 1;

	printf("%i \n", a);
	printf("%i \n", b);

	for(int x = 2; x<=10; x++)
	{
		c=a+b;
		printf("%i \n", c);
		a=b;
		b=c;

	}
	return 0;
}