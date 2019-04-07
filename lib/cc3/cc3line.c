/*
 * cc3line.c
 *
 *  Created on: Mar 30, 2019
 *      Author: jmiller
 */

#include "os.h"
#include "cc3hw.h"
#include "cc3gfx.h"
#include "cc3util.h"
#include "cc3raster.h"

extern GfxState gfx;

void line(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t clr) {
    int16_t stepX = x0 < x1 ? 1 : -1;
    int16_t stepY = y0 < y1 ? (int16_t) gfx.bytes_per_row : -(int16_t) gfx.bytes_per_row;
    const int16_t dx = abs(x1 - x0);
    const int16_t dy = abs(y1 - y0);

    rasterPos(x0, y0);
    rasterColor(clr);
    int count = max(dx, dy);
    int16_t err = dx - dy;
    disableInterrupts();
    const uint8_t oldPage = *PAGE_SELECT;
    do {
        const uint16_t offset = gfx.base_y_offset + (gfx.rasterX >> 1); // TODO: other bit depths

        *PAGE_SELECT = gfx.base_page + (uint8_t) (offset >> PAGE_BITS);
        uint8_t* ptr = (uint8_t*) PAGE_WINDOW + (offset & 0x1fff);
        *ptr = (*ptr & (~gfx.pixel_mask)) | (gfx.color & gfx.pixel_mask);

        int16_t e2 = err << 1;
        if (e2 < dx) {
            err += dx;
            gfx.base_y_offset += stepY; // inline rasterInc/DecY
        }
        if (e2 > -dy) {
            err -= dy;
            gfx.rasterX += stepX;
            gfx.pixel_mask = ~gfx.pixel_mask; // inline rasterInc/DecX
        }
    } while (count--);
    *PAGE_SELECT = oldPage;
    enableInterrupts();
}
