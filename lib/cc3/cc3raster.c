/*
 * cc3raster.c
 *
 *  Created on: Apr 1, 2019
 *      Author: jmiller
 */

#include "os.h"
#include "cc3hw.h"
#include "cc3gfx.h"
#include "cc3raster.h"
#include "cc3util.h"

extern GfxState gfx;

void rasterPos(int16_t x, int16_t y) {
    gfx.base_y_offset = y * gfx.bytes_per_row;
    gfx.rasterX = x;
    gfx.rasterY = y;
    gfx.pixel_mask = (uint8_t) x & 1 ? 0x0f : 0xf0; // TODO: other depths
}

void rasterColor(uint8_t color) {
    // Fill all bits with current color up front to avoid shifting later
    gfx.color = (color << 4) | (color & 0x0f); // TODO: handle other bit depths
}

void rasterSpan(int16_t count) {
    disableInterrupts();
    const uint8_t mmuSave = *PAGE_SELECT;
    if (count < 0) {
        gfx.rasterX += count;
        count = -count;
    }
    while (count > 0) {
        const uint16_t rasterStart = gfx.base_y_offset + (gfx.rasterX >> 1);
        const uint8_t* ptr = (uint8_t*) PAGE_WINDOW + (rasterStart & 0x1fff);
        *PAGE_SELECT = gfx.base_page + (uint8_t) (rasterStart >> 13);

        if ((uint8_t) gfx.rasterX & 1) {
            *ptr = (*ptr & 0xf0) | (gfx.color & 0x0f);
            gfx.rasterX++;
            count--;
        } else if (count > 1) {
            uint16_t n = min(count >> 1, PAGE_SIZE - (rasterStart & 0x1fff));
            fmemset(ptr, gfx.color, n);
            uint16_t nPixels = n << 1;
            gfx.rasterX += nPixels;
            count -= nPixels;
        } else if (count == 1) {
            *ptr &= 0x0f;
            *ptr |= gfx.color & 0xf0;
            gfx.rasterX++;
            count--;
        }
    }

    *PAGE_SELECT = mmuSave;
    enableInterrupts();
}

void rasterSet() {
    disableInterrupts();
    const uint16_t offset = gfx.base_y_offset + (gfx.rasterX >> 1); // TODO: other bit depths
    const uint8_t oldPage = *PAGE_SELECT;

    *PAGE_SELECT = gfx.base_page + (uint8_t) (offset >> PAGE_BITS) ;
    uint8_t* ptr = (uint8_t*) PAGE_WINDOW + (offset & 0x1fff);
    *ptr = (*ptr & (~gfx.pixel_mask)) | (gfx.color & gfx.pixel_mask);
    *PAGE_SELECT = oldPage;
    enableInterrupts();
}

void rasterIncX() {
    gfx.rasterX++;
    gfx.pixel_mask = ~gfx.pixel_mask; // TODO: Handle other depths
}

void rasterDecX() {
    gfx.rasterX--;
    gfx.pixel_mask = ~gfx.pixel_mask; // TODO: Handle other depths
}

void rasterIncY() {
    gfx.base_y_offset += gfx.bytes_per_row;
}

void rasterDecY() {
    gfx.base_y_offset -= gfx.bytes_per_row;
}
