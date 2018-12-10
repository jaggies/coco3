#include <cmoc.h>
#include <coco.h>

#define HS320x192x4 1
#define HS320x192x16 2
#define HS640x192x2 3
#define HS640x192x4 4

typedef int16_t fract;

static fract fraction = 10;
static fract mask = (((fract)(1) << fraction) - 1);
static fract two = ((fract)(2) << fraction);
static fract four = ((fract)(4) << fraction);

fract xres = 320;
fract yres = 192;

int maxCount = 16; // max printable ASCII letter = 127

float toFloat(fract value) {
    return (float) (value >> fraction) + (float)(value & mask) / mask;
}

fract mult(fract a, fract b) {
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
            uint8_t count = 0;
            fract dist;
            fract zr2 = 0; // initial condition = mult(zr, zr) = 0
            fract zi2 = 0; // initial condition = mult(zi, zr) = 0
            do {
                // z = z^2 + c
                fract tr = zr2 - zi2 + cr;
                fract ti = mult(zr << 1, zi) + ci;
                zr = tr; zi = ti;
                zr2 = mult(zr, zr);
                zi2 = mult(zi, zi);
            } while (count++ < maxCount && (zr2 + zi2) < four);
			hset(i, j, (count-1)%16);
        }
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
            uint8_t count = 0;
            do {
                // z = z^2 + c
                // (zr + zi)*(zr + zi) = zr*zr + 2*zr*zi - zi*zi
                float tr = zr * zr - zi * zi;
                float ti = 2.0f * zr * zi;
                zr = tr + cr;
                zi = ti + ci;
            } while ((count++ < maxCount) && (zr*zr + zi*zi) < 4.0f);
			hset(i, j, (count-1)%16);
        }
    }
}

// Enables 6309 Native mode for higher speed
void set6309Native() {
    asm {
        ldmd  #$01
    }
}

int main()
{
	set6309Native();
	initCoCoSupport();
	setHighSpeed(1);
	for (uint8_t i = 0; i < 16; i++) {
		uint8_t red = 3*(i >> 3);
		uint8_t grn = (i >> 1) & 3;
		uint8_t blu = 3*(i & 1);
		paletteRGB(i, red, grn, blu);
	}
    hscreen(HS320x192x16);
	doMandleInt(-2.0f, 1.0f, -1.25f, 1.25f);
    doMandelFloat(-2.0f, 1.0f, -1.25f, 1.25f);
	return 0;
}
