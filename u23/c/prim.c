#include <stdio.h>

int main(void)
{

	for(int x=1; x<=100; x++)
	{
		_Bool prim =1;
			for(int y=2; y<=x/2; y++)
			{
				if(x%y==0)
				{
					prim = 0;
				}
			}
		if(prim)
		{
			printf("%i\n", x);
		}
	}

	return 0;
}