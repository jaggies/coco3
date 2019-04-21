/*
 * palette.c
 *
 *  Created on: Feb 12, 2019
 *      Author: jmiller
 */

#include "os.h"
#include "cc3hw.h"
#include "cc3gfx.h"
#include "cc3line.h"
#include "cc3rect.h"
#include "cc3raster.h"

#define WIDTH 320
#define HEIGHT 200
#define DEPTH 4

#define HSIZE 50
#define VSIZE 50

void simpleRGB() {
    for (uint8_t i = 0; i < 4; i++) {
        paletteRGB(i, i, 0, 0); // red
        paletteRGB(i+4, 0, i, 0); // green
        paletteRGB(i+8, 0, 0, i); // blue
        paletteRGB(i+12, i, i, i); // white
    }
}

int16_t myrandom() {
    return rand() | (rand() >> 1);
}

int main(int argc, char** argv) {
    /* Speedups */
    set6309Native();
    initCoCoSupport();
    setHighSpeed(1);

    /* Graphics */
    setMode(WIDTH, HEIGHT, DEPTH);
    simpleRGB();
    clear(0x7); // white

    /* Draw pixels */
    const int height = getHeight();
    const int width = getWidth();

    /* Check for proper 1st pixel setting */
    for (int8_t i = 0; i < 32; i++) {
        int x0 = i;
        int x1 = x0 + HSIZE;
        int y0 = i;
        int y1 = y0 + VSIZE;
        rasterColor(i);
        rect(x0, y0, x1, y1, true);
    }

    /* Check for proper last pixel setting */
    for (int8_t i = 31; i >= 0; i--) {
        int x0 = i + HSIZE + 16;
        int x1 = x0 + HSIZE;
        int y0 = i + VSIZE + 16;
        int y1 = y0 + VSIZE;
        rasterColor(i);
        rect(x0, y0, x1, y1, true);
    }

    sleep(5);
    int count = 1000;
    while (count--) {
        int x0 = myrandom() % WIDTH;
        int x1 = myrandom() % WIDTH;
        int y0 = myrandom() % HEIGHT;
        int y1 = myrandom() % HEIGHT;
        rasterColor((uint8_t)myrandom() & 0x0f);
        rect(x0, y0, x1, y1, true);
    }

    return 0;
}
