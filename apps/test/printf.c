#include "os.h"

int main()
{
	printf("Hello, coco!\n");
#ifdef COCO
	printf("Max memory = %d bytes\n", sbrkmax());
#endif
	printf("sizeof(int) = %d\n", (int) sizeof(int));
	printf("sizeof(long int) = %d\n", (int) sizeof(long));
	printf("sizeof(long long) = %d\n", (int) sizeof(long long));
	long x = 0xffff;
	long y = 0xffff;
	printf("x * y = %lx\n", x * y);
	long a = 0x0001ffff;
	long b = 0x0002ffff;
	long result = a * b;
	printf("a * b = %lx\n", result);
	return 0;
}
