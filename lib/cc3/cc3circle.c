/*
 * cc3circle.c
 *
 *  Created on: Apr 7, 2019
 *      Author: jmiller
 */

#include "os.h"
#include "cc3gfx.h"
#include "cc3raster.h"
#include "cc3circle.h"

void circle(int16_t xc, int16_t yc, int16_t radius, uint8_t clr) {
    int16_t x = 0, y = radius;
    int16_t d = 3 - 2 * radius;
    rasterColor(clr);
    do {
        rasterPos(xc + x, yc + y); rasterSet();
        rasterPos(xc - x, yc + y); rasterSet();
        rasterPos(xc + y, yc + x); rasterSet();
        rasterPos(xc - y, yc + x); rasterSet();
        rasterPos(xc + x, yc - y); rasterSet();
        rasterPos(xc - x, yc - y); rasterSet();
        rasterPos(xc + y, yc - x); rasterSet();
        rasterPos(xc - y, yc - x); rasterSet();
        x++;
        if (d > 0) {
            y--;
            d = d + 4 * (x - y) + 10;
        } else {
            d = d + 4 * x + 6;
        }
    } while (y >= x);
}


