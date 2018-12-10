#include <coco.h>

int main()
{
	printf("Hello, coco!\n");
	printf("sizeof(int) = %d\n", sizeof(int));
	printf("sizeof(long int) = %d\n", sizeof(long));
	printf("sizeof(long long) = %d\n", sizeof(long long));
	long x = 0xffff;
	long y = 0xffff;
	printf("x*y = %lx\n", x * y);
	long a = 0x100000;
	long b = 0x200000;
	long result = a * b;
	printf("a * b = %lx\n", result);
	return 0;
}
