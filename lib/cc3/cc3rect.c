/*
 * cc3rect.c
 *
 *  Created on: Mar 30, 2019
 *      Author: jmiller
 */

#include "os.h"
#include "cc3gfx.h"
#include "cc3util.h"
#include "cc3rect.h"
#include "cc3line.h"
#include "cc3raster.h"

void rect(int16_t x0, int16_t y0, int16_t x1, int16_t y1, bool fill)
{
    if (fill) {
        if (x0 > x1) {
            int16_t tmp = x0; x0 = x1; x1 = tmp;
        }
        if (y0 > y1) {
            int16_t tmp = y0; y0 = y1; y1 = tmp;
        }
        int16_t dx = x1 - x0;
        do {
            rasterPos(x0, y0);
            rasterSpan(dx);
            y0++;
        } while (y0 < y1);
    } else { // Draw outline
        line(x0, y0, x1, y0);
        line(x0, y1, x1, y1);
        line(x0, y0, x0, y1);
        line(x1, y0, x1, y1);
    }
}


