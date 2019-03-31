#include "os.h"
#include "fixed.h"

#define XRES 78
#define YRES 24

int MAXCOUNT = 93; // max printable ASCII letter = 127
const fixed c_four = 4 << fraction;

void doMandleInt(float xmin, float xmax, float ymin, float ymax) {
    const fixed cr_min = fromFloat(xmin);
    const fixed cr_max = fromFloat(xmax);
    const fixed ci_min = fromFloat(ymin);
    const fixed ci_max = fromFloat(ymax);

    const fixed cr_delta = (cr_max - cr_min) / (XRES-1);
    const fixed ci_delta = (ci_max - ci_min) / (YRES-1);

    fixed ci = ci_min;
    for (int j = 0; j < YRES; j++) {
        fixed cr = cr_min;
        for (int i = 0; i < XRES; i++) {
            fixed zr = 0;
            fixed zi = 0;
            fixed zr2 = 0; // initial condition = fmult(zr, zr) = 0
            fixed zi2 = 0; // initial condition = fmult(zi, zr) = 0
            uint8_t count = 0;
            do {
                // z = z^2 + c
                fixed tr = zr2 - zi2 + cr;
                fixed ti = fmult(zr << 1, zi) + ci;
                zr = tr; zi = ti;
                zr2 = fmult(zr, zr);
                zi2 = fmult(zi, zi);
            } while (count++ < MAXCOUNT && (zr2 + zi2) < c_four);
            putchar(' ' + count);
            cr += cr_delta;
        }
        putchar('\n');
        ci += ci_delta;
    }
}

void doMandelFloat(float xmin, float xmax, float ymin, float ymax) {
    float cr_delta = (xmax - xmin)/(XRES-1);
    float ci_delta = (ymax - ymin)/(YRES-1);

    float ci = ymin;
    for (int j = 0; j < YRES; j++, ci += ci_delta) {
        float cr = xmin;
        for (int i = 0; i < XRES; i++, cr += cr_delta) {
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
            } while ((count++ < MAXCOUNT) && (zr*zr + zi*zi) < 4.0f);
            putchar(' ' + count);
        }
        putchar('\n');
    }
}

int main()
{
    printf("sizeof(fixed) = %d\n", (int) sizeof(fixed));
    printf("two = %f\n", toFloat(c_two));
    printf("four = %f\n", toFloat(c_two << 1));
    printf("two * two = %f\n", toFloat(fmult(c_two, c_two)));
    printf("two + four = %f\n", toFloat(c_two + c_four));
	//setHighSpeed(1);
    doMandleInt(-2.0f, 1.0f, -1.25f, 1.25f);
    doMandelFloat(-2.0f, 1.0f, -1.25f, 1.25f);
	return 0;
}
