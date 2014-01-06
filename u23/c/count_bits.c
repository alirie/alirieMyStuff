#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	char str[10];
	fgets(str,10,stdin);
	int x = atoi(str);
	int y = 0;

	//00000001 & 00000011 = 00000001

	for(int i = 0; i < 32; i++)
	{
		y += x & 1;
		x >>= 1;
	}
	printf("%i\n", y);

	return 0;
}