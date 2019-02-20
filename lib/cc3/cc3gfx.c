/*
 * cc3gfx.c
 *
 *  Created on: Feb 19, 2019
 *      Author: jmiller
 */

#include "os.h"
#include "cc3hw.h"
#include "cc3gfx.h"

// TODO: Define these by graphics mode...
#define WIDTH 320
#define HEIGHT 200
#define BPP 4
#define PIXELSPERBYTE (8/BPP)
#define PAGEMASK 8191

static uint16_t widthPixels = WIDTH;
static uint16_t heightPixels = HEIGHT;
static uint16_t widthBytes = WIDTH / PIXELSPERBYTE;
static uint8_t* mmuMemC000 = (uint8_t*) 0xc000; // RAM mapped into cartridge space

// Constructs a palette entry of RGBRGB in the hardware format for Coco3.
uint8_t toPalette(uint8_t r, uint8_t g, uint8_t b) {
    uint8_t result = 0;
    result = (result | ((r >> 1) & 1)) << 1;
    result = (result | ((g >> 1) & 1)) << 1;
    result = (result | ((b >> 1) & 1)) << 1;
    result = (result | (r & 1)) << 1;
    result = (result | (g & 1)) << 1;
    result = (result | (b & 1));
    return result;
}

void setPixel(uint16_t x, uint16_t y, uint8_t clr) {
    disableInterrupts();
    // TODO: handle other pixel sizes
    clr &= 0x0f;
    long pixelByteAddr = *(uint16_t*) vertOffset;
    pixelByteAddr <<= 3; // vert offset only represents bits [18:3]
    pixelByteAddr += y * widthBytes + x / PIXELSPERBYTE;
    long page = pixelByteAddr >> 13; // 8kB page number

    const uint8_t tmp = *mmuEntryC000; // save current mapping for 0xc000 - 0xdfff
    *mmuEntryC000 = (uint8_t) page;
    long offset = page & PAGEMASK;
    *(mmuMemC000 + offset) &= x & 1 ? 0x0f : 0xf0; // mask old value off TODO: other pixel sizes
    *(mmuMemC000 + offset) |= x & 1 ? (clr << 4) : clr;
    *mmuEntryC000 = tmp; // set it back

    enableInterrupts();
}
