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

extern GfxState gfx;

void rect(int16_t x0, int16_t y0, int16_t x1, int16_t y1, bool fill)
{
    if (fill) {
        int16_t xmin = min(x0, x1);
        int16_t xmax = max(x0, x1);
        int16_t ymin = min(y0, y1);
        int16_t ymax = max(y0, y1);
        do {
            fillPixels(xmin, ymin, gfx.color, xmax - xmin);
            ymin++;
        } while (ymin < ymax);
    } else {
        line(x0, y0, x1, y0);
        line(x0, y1, x1, y1);
        line(x0, y0, x0, y1);
        line(x1, y0, x1, y1);
    }
}


