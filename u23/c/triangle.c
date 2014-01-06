#include <stdio.h>
float triangle(float g, float h)
{
	return g*h/2;
}

int main(void)
{
	float g = 2;
	float h = 4;

	printf("%.2f\n",triangle(g,h));
	return 0;
}