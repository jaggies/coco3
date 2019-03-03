#include "os.h"
#include "cc3hw.h"

int main()
{
    int16_t a16 = -1;
    int16_t b16 = 1;
    int32_t c = 0xffffffffL;
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

	c = fastmult(a16, b16);
	int32_t d = a16*b16;
	printf("FASTMULT: %x x %x = %lx\n", a16, b16, c);
	printf("CMOCMULT: %x x %x = %lx\n", a16, b16, d);
	return 0;
}
