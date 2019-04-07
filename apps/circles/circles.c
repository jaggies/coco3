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

#define WIDTH 320
#define HEIGHT 225
#define DEPTH 4

void simpleRGB() {
    for (uint8_t i = 0; i < 16; i++) {
        uint8_t red = (i & 1) ? 3 : 0;
        uint8_t grn = (i >> 1) & 3;
        uint8_t blu = ((i >> 3) & 1) ? 3 : 0;
        setPalette(i, red, grn, blu);
    }
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

    uint8_t clr = 1;

    while (1) {
        int16_t xc = width/2;
        int16_t yc = height/2;
        circle(xc, yc, clr, clr++);
    }

    return 0;
}
