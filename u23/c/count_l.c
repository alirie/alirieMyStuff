#include <stdio.h>
int count(char y)
{
	int x=0;
	char c;
	while((c = getchar()) != EOF)
	{
		if(c==y) x++;

	}
	//printf("%i\n", x);
	return x;
}

int main(void)
{
	char a = 'l';
	printf("%i\n", count(a));
	return 0;
}