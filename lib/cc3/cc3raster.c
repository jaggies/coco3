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

void rasterPos(int x, int y) {
    gfx.base_y_offset = y * gfx.bytes_per_row;
    gfx.rasterX = x;
    gfx.rasterY = y;
}

void rasterColor(uint8_t color) {
    // Fill all bits with current color up front to avoid shifting later
    gfx.color = (color << 4) | (color & 0x0f); // TODO: handle other bit depths
}

void rasterSet() {
    disableInterrupts();
    const uint16_t offset = gfx.base_y_offset + (gfx.rasterX >> 1);
    const uint32_t addr = gfx.base_addr + offset;
    const uint8_t oldPage = *PAGE_SELECT;

    // TODO: Handle other bit depths
    uint8_t mask = gfx.rasterX & 1 ? 0x0f : 0xf0;

    *PAGE_SELECT = (uint8_t) (addr >> 13);
    uint8_t* ptr = (uint8_t*) PAGE_WINDOW + (offset & 0x1fff);
    *ptr = (*ptr & (~mask)) | (gfx.color & mask);
    *PAGE_SELECT = oldPage;
    enableInterrupts();
}

void rasterIncX() {
    gfx.rasterX++;
}

void rasterDecX() {
    gfx.rasterX--;
}

void rasterIncY() {
    gfx.base_y_offset += gfx.bytes_per_row;
}

void rasterDecY() {
    gfx.base_y_offset -= gfx.bytes_per_row;
}
