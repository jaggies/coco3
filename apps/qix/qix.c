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
#define HEIGHT 225
#define DEPTH 4
#define HISTORY 16 // number of lines to keep
#define BG 0x0

void simpleRGB() {
    for (uint8_t i = 0; i < 16; i++) {
        uint8_t red = (i & 1) ? 3 : 0;
        uint8_t grn = (i >> 1) & 3;
        uint8_t blu = ((i >> 3) & 1) ? 3 : 0;
        setPalette(i, red, grn, blu);
    }
}

// zeroed by default
int hx0[HISTORY];
int hy0[HISTORY];
int hx1[HISTORY];
int hy1[HISTORY];
int pal[HISTORY];

int main(int argc, char** argv) {
    /* Speedups */
    set6309Native();
    initCoCoSupport();
    setHighSpeed(1);

    /* Graphics */
    setMode(WIDTH, HEIGHT, DEPTH);
    simpleRGB();
    clear(BG);

    /* Draw pixels */
    const int height = getHeight();
    const int width = getWidth();

    int vx0 = 3;
    int vy0 = 2;
    int vx1 = 4;
    int vy1 = 3;
    int* x0 = &hx0[HISTORY-1];
    int* y0 = &hy0[HISTORY-1];
    int* x1 = &hx1[HISTORY-1];
    int* y1 = &hy1[HISTORY-1];
    *x0 = *x1 = width / 2;
    *y0 = height/2 - 50;
    *y1 = height/2 + 50;
    uint8_t count = 0;

    while (1) {
        for (uint8_t i = 0; i < HISTORY-1; i++) {
            hx0[i] = hx0[i+1];
            hy0[i] = hy0[i+1];
            hx1[i] = hx1[i+1];
            hy1[i] = hy1[i+1];
            pal[i] = pal[i+1];
        }

        *x0 += vx0;
        *y0 += vy0;
        *x1 += vx1;
        *y1 += vy1;
        if (*x0 < 0 || *x0 > width) {
            vx0 = -vx0;
            *x0 += vx0;
        }
        if (*y0 < 0 || *y0 > height) {
            vy0 = -vy0;
            *y0 += vy0;
        }
        if (*x1 < 0 || *x1 > width) {
            vx1 = -vx1;
            *x1 += vx1;
        }
        if (*y1 < 0 || *y1 > height) {
            vy1 = -vy1;
            *y1 += vy1;
        }

        // Erase the last line
        rasterColor(0x00);
        line(hx0[0], hy0[0], hx1[0], hy1[0]);

        // Draw the new line
        pal[count] = ((count % 15) + 1); // don't allow black
        rasterColor(pal[count]);
        line(*x0, *y0, *x1, *y1);

        count++;
    }

    return 0;
}
