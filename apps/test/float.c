#include <coco.h>
#include "math.h"

int main(int argc, char**argv)
{
	float x = 3.5f;
	float y = 2.0f;
	printf("sizeof(short)=%d\n", sizeof(short));
	printf("sizeof(int)=%d\n", sizeof(int));
	printf("sizeof(long)=%d\n", sizeof(long));
	printf("sizeof(float)=%d\n", sizeof(float));
	printf("x*y=%f\n", x * y);
	printf("sqrt(%f)=%f\n", x, sqrt(x));
	printf("pow(%f,%f)=%f\n", x, y, pow(x,y));
	return 0;
}
