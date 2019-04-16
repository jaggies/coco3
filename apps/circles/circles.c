/*
 * palette.c
 *
 *  Created on: Feb 12, 2019
 *      Author: jmiller
 */

#include "os.h"
#include "cc3hw.h"
#include "cc3gfx.h"
#include "cc3circle.h"
#include "cc3raster.h"

#define WIDTH 320
#define HEIGHT 225
#define DEPTH 4

#define N 20

void simpleRGB() {
    for (uint8_t i = 0; i < 16; i++) {
        uint8_t red = (i & 1) ? 3 : 0;
        uint8_t grn = (i >> 1) & 3;
        uint8_t blu = ((i >> 3) & 1) ? 3 : 0;
        setPalette(i, red, grn, blu);
    }
}

int16_t myrandom() {
    return (rand() >> 1) ^ rand();
}

int main(int argc, char** argv) {
    /* Speedups */
    set6309Native();
    initCoCoSupport();
    setHighSpeed(1);

    /* Graphics */
    setMode(WIDTH, HEIGHT, DEPTH);
    simpleRGB();
    clear(0x8); // blue

    /* Draw pixels */
    const int height = getHeight();
    const int width = getWidth();

    int count = 1000;
    while (count--) {
        int16_t xc, yc, r;
        do {
            xc = myrandom() % width;
            yc = myrandom() % height;
            r = myrandom() % (width>>2);
        } while (xc-r < 0 || yc-r < 0 || xc+r > width || yc+r > height);
        rasterColor((uint8_t) (myrandom()));
        circle(xc, yc, r, true);
    }

    clear(0x8); // blue

    uint8_t clr = 1;
    count = 100;
    while (count--) {
        int16_t xc = width/2;
        int16_t yc = height/2;
        rasterColor(clr);
        circle(xc, yc, clr++, false);
    }

    return 0;
}
