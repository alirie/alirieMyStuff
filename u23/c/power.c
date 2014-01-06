#include <stdio.h>

int main(void)
{
	
	for(int i=2; i<=10; i++)
	{
		int x=1;
		for(int j=1; j<=5; j++)
		{
			x*=i;
		}
		printf("%i\n",x);
	}

	return 0;
}