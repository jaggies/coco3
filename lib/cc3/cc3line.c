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

void line(int16_t x0, int16_t y0, int16_t x1, int16_t y1) {
    // Always render in positive Y direction
    if (y0 > y1) {
        int16_t tmp = x0; x0 = x1; x1 = tmp;
        tmp = y0; y0 = y1; y1 = tmp;
    }
    const int16_t stepX = x0 < x1 ? 1 : -1;
    const int16_t stepY = gfx.bytes_per_row; // always incrementing
    const int16_t dx = abs(x1 - x0);
    const int16_t dy = y0 - y1; // dy is always negative

    rasterPos(x0, y0);
    int count = max(dx, -dy);
    int16_t err = dx + dy;
    disableInterrupts();
    const uint8_t oldPage = *PAGE_SELECT;
    do {
        const uint16_t offset = gfx.base_y_offset + (gfx.rasterX >> 1); // TODO: other bit depths

        *PAGE_SELECT = gfx.base_page + (uint8_t) (offset >> PAGE_BITS);
        uint8_t* ptr = (uint8_t*) PAGE_WINDOW + (offset & PAGE_MASK);
        gfx.pixel_mask = gfx.masks[(uint8_t) gfx.rasterX & 7];
        *ptr = (*ptr & (~gfx.pixel_mask)) | (gfx.color & gfx.pixel_mask);

        int16_t e2 = err << 1;
        if (e2 < dx) {
            err += dx;
            gfx.base_y_offset += stepY; // inline rasterInc/DecY
        }
        if (e2 > dy) {
            err += dy;
            gfx.rasterX += stepX;
        }
    } while (count--);
    *PAGE_SELECT = oldPage;
    enableInterrupts();
}
