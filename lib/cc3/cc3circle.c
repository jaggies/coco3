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

// Draw circle using the midpoint algorithm.
void circle(int16_t xc, int16_t yc, int16_t radius, uint8_t clr) {
    if (radius <= 0) {
        setPixel(xc, yc, clr);
        return;
    }

    int16_t x = radius, y = 0;
    int16_t midpoint = 1 - radius;
    do {
        setPixel(x + xc, y + yc, clr);
        setPixel(-x + xc, y + yc, clr);
        setPixel(x + xc, -y + yc, clr);
        setPixel(-x + xc, -y + yc, clr);
        setPixel(y + xc, x + yc, clr);
        setPixel(-y + xc, x + yc, clr);
        setPixel(y + xc, -x + yc, clr);
        setPixel(-y + xc, -x + yc, clr);

        y++;
        if (midpoint <= 0) { // midpoint inside or on the perimeter
            midpoint = midpoint + (y << 1) + 1;
        } else { // midpoint outside the perimeter
            x--;
            midpoint = midpoint + ((y - x) << 1) + 1;
        }
    } while (x >= y);
}
