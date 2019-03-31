#include "os.h"
#include "math.h"

#ifndef COCO
extern double pow(double, double);
extern double sqrt(double);
#endif

int main(int argc, char**argv)
{
	float x = 3.5f;
	float y = 2.0f;
	printf("sizeof(short)=%d\n", (int) sizeof(short));
	printf("sizeof(int)=%d\n", (int) sizeof(int));
	printf("sizeof(long)=%d\n", (int) sizeof(long));
	printf("sizeof(float)=%d\n", (int) sizeof(float));
	printf("x*y=%f\n", x * y);
	printf("sqrt(%f)=%f\n", x, sqrt(x));
	printf("pow(%f,%f)=%f\n", x, y, pow(x,y));


	float a = 1.0f;
	for (int i = 0; i < 129; i++) {
		printf("%f %08x\n", a, *(int*)&a);
		a *= 2.0f;
	}

	return 0;
}
