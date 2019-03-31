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

#define WIDTH 320
#define HEIGHT 200
#define VBLANK (262 - HEIGHT)
#define DEPTH 4
#define SIZE 50

void simpleRGB() {
    for (uint8_t i = 0; i < 4; i++) {
        paletteRGB(i, i, 0, 0); // red
        paletteRGB(i+4, 0, i, 0); // green
        paletteRGB(i+8, 0, 0, i); // blue
        paletteRGB(i+12, i, i, i); // white
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
    clear(0);

    /* Draw pixels */
    const int height = getHeight();
    const int width = getWidth();

    rect(WIDTH/2-SIZE, HEIGHT/2-SIZE, WIDTH/2 + SIZE, HEIGHT/2 + SIZE, 0x0e, false);

    for (int i = 0; i < 4; i++) {
        int x = WIDTH/2 + (i & 1 ? -SIZE : SIZE);
        int y = HEIGHT/2 + ((i>>1) & 1 ? -SIZE : SIZE);
        line(WIDTH/2, HEIGHT/2, x, y, (uint8_t) (i * 4 + 3));
    }

    sleep(4);

    while (1)
    {
        uint8_t clr = (uint8_t) (rand() & 0x0f);
        line(rand() % width, rand() % height, rand() % width, rand() % height, clr);
    }

    return 0;
}
