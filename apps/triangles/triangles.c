/*
 * palette.c
 *
 *  Created on: Feb 12, 2019
 *      Author: jmiller
 */

#include "os.h"
#include "cc3hw.h"
#include "cc3gfx.h"
#include "cc3triangle.h"

#define WIDTH 320
#define HEIGHT 200
#define VBLANK (262 - HEIGHT)
#define DEPTH 4
#define SIZE 50

void simpleRGB() {
    for (uint8_t i = 0; i < 16; i++) {
        uint8_t red = (i & 1) ? 3 : 0;
        uint8_t grn = (i >> 1) & 3;
        uint8_t blu = ((i >> 3) & 1) ? 3 : 0;
        setPalette(i, red, grn, blu);
    }
}

int myrand() {
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
    clear(0);

    /* Draw pixels */
    const int height = getHeight();
    const int width = getWidth();
    int v[3][2];
    while (1) {
        uint8_t clr = (uint8_t) (myrand() & 0x0f);
        for (int i = 0; i < 3; i++) {
            v[i][0] = myrand() % width;
            v[i][1] = myrand() % height;
        }
        triangle(v[0], v[1], v[2], clr);
    }

    return 0;
}
