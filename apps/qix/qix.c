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
#define HISTORY 32 // number of lines to keep
#define BG 0x0

void simpleRGB() {
    for (uint8_t i = 0; i < 16; i++) {
        uint8_t red = (i & 1) ? 3 : 0;
        uint8_t grn = (i >> 1) & 3;
        uint8_t blu = ((i >> 3) & 1) ? 3 : 0;
        setPalette(i, red, grn, blu);
    }
}

typedef struct _Point {
    int x;
    int y;
} Point;

// zeroed by default
Point p0[HISTORY];
Point p1[HISTORY];

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

    uint8_t head = HISTORY-1;
    uint8_t tail = 0;

    p0[head].x = width / 2;
    p1[head].x = width / 2;
    p0[head].y = height / 2 - 50;
    p1[head].y = height / 2 + 50;
    uint8_t count = 0;

    const int nbytes = (HISTORY-1) * sizeof(int);
    while (1) {
        // Erase the oldest line
        rasterColor(0x00);
        line(p0[tail].x, p0[tail].y, p1[tail].x, p1[tail].y);

        // Draw the new line
        rasterColor((count++ % 15) + 1);
        line(p0[head].x, p0[head].y, p1[head].x, p1[head].y);

        int oldhead = head;

        head = (head + 1) % HISTORY;
        tail = (tail + 1) % HISTORY;

        p0[head].x = p0[oldhead].x + vx0;
        p0[head].y = p0[oldhead].y + vy0;
        p1[head].x = p1[oldhead].x + vx1;
        p1[head].y = p1[oldhead].y + vy1;

        if (p0[head].x < 0 || p0[head].x > width) {
            vx0 = -vx0;
            p0[head].x += vx0 << 1;
        }
        if (p0[head].y < 0 || p0[head].y > height) {
            vy0 = -vy0;
            p0[head].y += vy0 << 1;
        }
        if (p1[head].x < 0 || p1[head].x > width) {
            vx1 = -vx1;
            p1[head].x += vx1 << 1;
        }
        if (p1[head].y < 0 || p1[head].y > height) {
            vy1 = -vy1;
            p1[head].y += vy1 << 1;
        }
    }

    return 0;
}
