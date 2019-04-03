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

    int x[N], y[N];

    for (int i = 0; i < N; i++) {
        x[i] = (width - height) / 2 + (int) (height * 0.5f * (1.0f + cos(2.0f * M_PI * i / (N - 1))));
        y[i] = (int) (height * 0.5f * (1.0f + sin(2.0f * M_PI * i / (N - 1))));
    }

    for (uint8_t clr = 1; clr < 16; clr++) {
        for (int n = 0; n < N; n++) {
            for (int m = n+1; m < N; m++) {
                line(x[n], y[n], x[m], y[m], clr);
            }
        }
    }

    return 0;
}
