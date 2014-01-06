#include <stdio.h>
void exchange(int * x, int * y)
{
	int help = *x;
	*x = *y;
	*y = help;
}
int main(void)
{
	int a[5] = {9, 17, 4, 108, 11};
	
	
	for(int i = 0; i<5; i++)
	{
		for(int j=0; j<5;  j++)
		{
			if(a[i]<a[j]) exchange(&(a[i]), &(a[j]));
		} 
	}

	for(int k = 0; k < 5; k++)
	{
		printf("%i\n", a[k]);
	}

	return 0;
}