#define EXP_A 184
#define EXP_C 16249 

union {
	float d;
	struct {
		#ifdef LITTLE_ENDIAN
		short j, i;
		#else
		short i, j;
		#endif
	} n;
} eco;

static float exp(float y) {
	eco.n.i = (int) (EXP_A*(y) + (EXP_C));
	eco.n.j = 0;
	return eco.d;
}

float log(float y) {
	int * nTemp = (int*)&y;
	y = (*nTemp) >> 16;
	return (y - EXP_C) / EXP_A;
}

float pow(float b, float p) {
	return exp(log(b) * p);
}
