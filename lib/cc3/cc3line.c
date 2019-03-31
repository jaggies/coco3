/*
 * cc3line.c
 *
 *  Created on: Mar 30, 2019
 *      Author: jmiller
 */

#include "os.h"
#include "cc3gfx.h"
#include "cc3util.h"
#include "cc3rect.h"

void line(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t clr) {
    const int16_t stepX = x0 < x1 ? 1 : -1;
    const int16_t stepY = y0 < y1 ? 1 : -1;
    const int16_t dx = abs(x1-x0);
    const int16_t dy = abs(y1-y0);
    int16_t err = dx - dy;

    // required to set first pixel
    bool xchanged = true;
    bool ychanged = true;

    int16_t span = 1;
    while (true) {
        const bool end = x0 == x1 && y0 == y1;
        if (dx > dy) {
            if (ychanged || end) {
//                rect(x0, y0, x0 - (span-1)*stepX, y0, clr, true);
                span = 0;
                ychanged = false;
            }
        } else {
            if (xchanged || end) {
//                rect(x0, y0, x0, y0 - (span-1)*stepY, clr, true);
                span = 0;
                xchanged = false;
            }
        }
        setPixel(x0,y0,clr);
        if (end) break;

        int16_t e2 = err << 1;
        if (e2 <  dx) {
           err += dx;
           y0 += stepY;
           ychanged = true;
        }
        if (e2 > -dy) {
           err -= dy;
           x0 += stepX;
           xchanged = true;
        }
        span++;
    }
}


