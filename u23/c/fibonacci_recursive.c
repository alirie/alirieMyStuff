#include <stdio.h>

int fibonacci(int x)
{
	if(x>2)
	{

		return fibonacci(x-1)+fibonacci(x-2);
	}
	if(x==0) return 0;
	return 1;
}
int main(void)
{
	

	for(int i=0; i<10; i++)
	{
		printf("%i\n",fibonacci(i));
	}
	
	return 0;
}