#include "os.h"

typedef int16_t fract;

static const fract fraction = 12;
static const fract mask = (((fract)(1) << fraction) - 1);
static fract two = ((fract)(2) << fraction);
static fract four = ((fract)(4) << fraction);

fract xres = 78;
fract yres = 24;

int maxCount = 93; // max printable ASCII letter = 127

float toFloat(fract value) {
    return (float) (value >> fraction) + (float)(value & mask) / mask;
}

fract fmult(fract a, fract b) {
    long long x = a;
    long long y = b;
    return (fract) (x * y >> fraction);
}

fract toFract(float value) {
    return (fract) (value * (1 << fraction));
}

void doMandleInt(float xmin, float xmax, float ymin, float ymax) {
    fract cr_min = toFract(xmin);
    fract cr_max = toFract(xmax);
    fract ci_min = toFract(ymin);
    fract ci_max = toFract(ymax);

    fract cr_delta = (cr_max - cr_min)/(xres-1);
    fract ci_delta = (ci_max - ci_min)/(yres-1);

    fract ci = ci_min;
    for (int j = 0; j < yres; j++, ci += ci_delta) {
        fract cr = cr_min;
        for (int i = 0; i < xres; i++, cr += cr_delta) {
            fract zr = 0;
            fract zi = 0;
            int count = 0;
            fract dist;
            fract zr2 = 0; // initial condition = fmult(zr, zr) = 0
            fract zi2 = 0; // initial condition = fmult(zi, zr) = 0
            do {
                // z = z^2 + c
                fract tr = zr2 - zi2 + cr;
                fract ti = fmult(zr << 1, zi) + ci;
                zr = tr; zi = ti;
                zr2 = fmult(zr, zr);
                zi2 = fmult(zi, zi);
            } while (count++ < maxCount && (zr2 + zi2) < four);
            putchar(' ' + count);
        }
        putchar('\n'); 
    }
}

void doMandelFloat(float xmin, float xmax, float ymin, float ymax) {
    float cr_delta = (xmax - xmin)/(xres-1);
    float ci_delta = (ymax - ymin)/(yres-1);

    float ci = ymin;
    for (int j = 0; j < yres; j++, ci += ci_delta) {
        float cr = xmin;
        for (int i = 0; i < xres; i++, cr += cr_delta) {
            float zr = 0;
            float zi = 0;
            int count = 0;
            do {
                // z = z^2 + c
                // (zr + zi)*(zr + zi) = zr*zr + 2*zr*zi - zi*zi
                float tr = zr * zr - zi * zi;
                float ti = 2.0f * zr * zi;
                zr = tr + cr;
                zi = ti + ci;
            } while ((count++ < maxCount) && (zr*zr + zi*zi) < 4.0f);
            putchar(' ' + count);
        }
        putchar('\n');
    }
}

int main()
{
    printf("sizeof(fract) = %d\n", (int) sizeof(fract));
    printf("sizeof(long long) = %d\n", (int) sizeof(long long));
    printf("two = %f\n", toFloat(two));
    printf("four = %f\n", toFloat(four));
    printf("two * four = %f\n", toFloat(fmult(two, four)));
    printf("two + four = %f\n", toFloat(two + four));
	//setHighSpeed(1);
    doMandleInt(-2.0f, 1.0f, -1.25f, 1.25f);
    doMandelFloat(-2.0f, 1.0f, -1.25f, 1.25f);
	return 0;
}
