/*
 * cc3raster.c
 *
 *  Created on: Apr 1, 2019
 *      Author: jmiller
 */

#include "os.h"
#include "cc3hw.h"
#include "cc3gfx.h"

extern GfxState gfx;

void rasterPos(int16_t x, int16_t y) {
    gfx.base_y_offset = y * gfx.bytes_per_row;
    gfx.rasterX = x;
    gfx.rasterY = y;
    gfx.pixel_mask = x & 1 ? 0x0f : 0xf0; // TODO: other depths
}

void rasterColor(uint8_t color) {
    // Fill all bits with current color up front to avoid shifting later
    gfx.color = (color << 4) | (color & 0x0f); // TODO: handle other bit depths
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
