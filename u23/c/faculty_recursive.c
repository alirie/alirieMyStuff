#include <stdio.h>

int faculty(int x)
{	
	if(x>1)
	{
		return x*faculty(x-1);
	}

	return 1;
}
int main(void)
{
	int x = 10;
	x=faculty(x);
	printf("%i\n", x);
	return 0;
}