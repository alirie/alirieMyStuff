#include <stdio.h>
#include <math.h>

double distance(double x1, double x2, double y1, double y2, double z1, double z2)
{
	return sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)+(z1-z2)*(z1-z2));
}

int main(void)
{
	double x1 = 1, x2 = 2, y1 = 3, y2 = 4, z1 = 5, z2 = 6;

	printf("%f\n", distance(x1, x2, y1, y2, z1, z2));
	return 0;
}