#include "os.h"
#include "fixed.h"
#include "cc3hw.h"
#include "cc3gfx.h"
#include "cc3raster.h"

#define XRES 320
#define YRES 200
#define DEPTH 4
#define MAXCOUNT 64

void doMandleInt(float xmin, float xmax, float ymin, float ymax) {
    const fixed cr_min = fromFloat(xmin);
    const fixed cr_max = fromFloat(xmax);
    const fixed ci_min = fromFloat(ymin);
    const fixed ci_max = fromFloat(ymax);

    const fixed cr_delta = (cr_max - cr_min) / (XRES-1);
    const fixed ci_delta = (ci_max - ci_min) / (YRES-1);

    const fixed c_four = c_two << 1;

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
            rasterColor((count-1) & 0x0f);
			setPixel(i, j);
			cr += cr_delta;
        }
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
            uint8_t count = 0;
            do {
                // z = z^2 + c
                // (zr + zi)*(zr + zi) = zr*zr + 2*zr*zi - zi*zi
                float tr = zr * zr - zi * zi;
                float ti = 2.0f * zr * zi;
                zr = tr + cr;
                zi = ti + ci;
            } while ((count++ < MAXCOUNT) && (zr*zr + zi*zi) < 4.0f);
            rasterColor((count-1) & 0x0f);
            setPixel(i, j);
        }
    }
}

int main()
{
	set6309Native();
	initCoCoSupport();
	setHighSpeed(1);
	setMode(XRES, YRES, DEPTH);
	clear(0x7);
	for (uint8_t i = 0; i < 16; i++) {
		uint8_t blu = i & 1 ? 3 : 0;
		uint8_t grn = (i >> 1) & 3;
		uint8_t red = (i >> 3) & 1 ? 3 : 0;
		setPalette(i, red, grn, blu);
	}
	doMandleInt(-2.0f, 1.0f, -1.25f, 1.25f);
    doMandelFloat(-2.0f, 1.0f, -1.25f, 1.25f);
	return 0;
}
