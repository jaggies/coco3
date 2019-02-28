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
#define BPP 4
#define PIXELSPERBYTE (8/BPP)
#define PAGEMASK 8191

#define CRES_1BPP 0
#define CRES_2BPP 1
#define CRES_4BPP 2

#define LPF_192 (0 << 5)
#define LPF_200 (1 << 5)
#define LPF_225 (3 << 5)

#define HRES7 (7 << 2) // 640x2bpp or 320x4bpp mode
#define HRES6 (6 << 2) // 512x2bpp or 256x4bpp mode
#define HRES5 (5 << 2) // 640x1bpp or 320x2bpp or 160x4bpp
#define HRES4 (4 << 2) // 512x1bpp or 256x2bpp
#define HRES2 (2 << 2) // 256x1bpp

static uint16_t widthPixels = 0;
static uint16_t heightPixels = 0;
static uint8_t bytesPerRow = 0; // TODO: use 16-bit math if 256-color modes ever are a thing
static uint16_t bufferSizeBytes = 0;
static uint8_t bpp = 0;
static uint32_t gfxBase;

static void setPixel1bpp(uint16_t x, uint16_t y, uint8_t clr);
static void setPixel2bpp(uint16_t x, uint16_t y, uint8_t clr);
static void setPixel4bpp(uint16_t x, uint16_t y, uint8_t clr);
static void setPixels1bpp(uint16_t x, uint16_t y, uint8_t* clr, uint16_t n);
static void setPixels2bpp(uint16_t x, uint16_t y, uint8_t* clr, uint16_t n);
static void setPixels4bpp(uint16_t x, uint16_t y, uint8_t* clr, uint16_t n);

void (*setPixel)(uint16_t x, uint16_t y, uint8_t clr);
void (*setPixels)(uint16_t x, uint16_t y, uint8_t clr, uint16_t n);

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

int setMode(uint16_t xres, uint16_t yres, uint8_t depth) {
    uint8_t vrr = 0; // video resolution register setting when done
    switch (depth) {
        case 1: bpp = depth; vrr |= CRES_1BPP; setPixel = setPixel1bpp; setPixels = setPixels1bpp;break;
        case 2: bpp = depth; vrr |= CRES_2BPP; setPixel = setPixel2bpp; setPixels = setPixels2bpp; break;
        case 4: bpp = depth; vrr |= CRES_4BPP; setPixel = setPixel4bpp; setPixels = setPixels4bpp; break;
        default: return 0;
    }
    switch (xres) {
        case 160: widthPixels = 160; vrr |= (depth == 4 ? HRES6 : 0); break;
        case 256: widthPixels = 256; vrr |= (depth == 4 ? HRES6 : depth == 2 ? HRES4 : depth == 1 ? HRES2 : 0); break;
        case 320: widthPixels = 320; vrr |= (depth == 4 ? HRES7 : depth == 2 ? HRES5 : 0); break;
        case 640: widthPixels = 640; vrr |= (depth == 2 ? HRES7 : depth == 1 ? HRES5 : 0); break;
        default: return 0;
    }
    switch (yres) {
        case 192: heightPixels = 192; vrr |= LPF_192; break;
        case 200: heightPixels = 200; vrr |= LPF_200; break;
        case 225: heightPixels = 225; vrr |= LPF_225; break;
        default: return 0;
    }
    bytesPerRow = (uint8_t) (xres * bpp >> 3);
    bufferSizeBytes = bytesPerRow * yres;
    *VIDEO_RES = vrr;
    *VIDEO_MODE = 0x80; // graphics mode, 60Hz
    *INIT0 = 0x4c; // Coco3 mode & MMU enabled
    *VERT_SCROLL = 0; // clear vertical scroll register
    setGraphicsBase(0x60000L); // set to default location

    return 1;
}

void setPalette(uint8_t index, uint8_t r, uint8_t g, uint8_t b) {
    *(PALETTE_BASE+(index&0xf)) = toPalette(r, g, b);
}

void clear(uint8_t color) {
    uint8_t pixels;
    switch (bpp) {
        case 1: pixels = color & 1;
                pixels |= pixels << 1;
                pixels |= pixels << 2;
                pixels |= pixels << 4;
                break;
        case 2: pixels = color & 3;
                pixels |= pixels << 2;
                pixels |= pixels << 4;
                break;
        case 4: pixels = color & 0x0f;
                pixels |= pixels << 4;
                break;
    }
    memset24(gfxBase, pixels, bufferSizeBytes);
}

void setPixel1bpp(uint16_t x, uint16_t y, uint8_t clr) {
    uint8_t mask;
    uint8_t bit;
    uint8_t shift;
    uint16_t byteOffset;
    bit = ~(uint8_t)x & 7; // pixels fill in from MSB right
    clr = (clr & 1) << bit;
    mask = ((uint8_t)1 << bit);
    byteOffset = (x >> 3) + y * bytesPerRow;
    memset1(gfxBase + byteOffset, clr, mask);
}

void setPixel2bpp(uint16_t x, uint16_t y, uint8_t clr) {
    uint8_t mask;
    uint8_t bit;
    uint8_t shift;
    uint16_t byteOffset;
    bit = ~(uint8_t)x & 3; // pixels fill in from MSB right
    shift = bit << 1;
    clr = (clr & 0x3) << shift;
    mask = (uint8_t) 0x3 << shift;
    byteOffset = (x >> 2) + y * bytesPerRow;
    memset1(gfxBase + byteOffset, clr, mask);
}

void setPixel4bpp(uint16_t x, uint16_t y, uint8_t clr) {
    uint8_t mask;
    uint16_t byteOffset;
    mask = x & 1 ? 0x0f : 0xf0;
    clr = x & 1 ? (clr & 0x0f) : ((clr & 0x0f) << 4);
    byteOffset = (x >> 1) + y * bytesPerRow;
    memset1(gfxBase + byteOffset, clr, mask);
}

void setPixels1bpp(uint16_t x, uint16_t y, uint8_t* clr, uint16_t n) {
    memcpy24(gfxBase + (x >> 3) + y * bytesPerRow, clr, n);
}

void setPixels2bpp(uint16_t x, uint16_t y, uint8_t* clr, uint16_t n) {
    memcpy24(gfxBase + (x >> 2) + y * bytesPerRow, clr, n);
}

void setPixels4bpp(uint16_t x, uint16_t y, uint8_t* clr, uint16_t n) {
    memcpy24(gfxBase + (x >> 1) + y * bytesPerRow, clr, n);
}

uint16_t packPixels(uint8_t* const in, uint8_t* out, uint16_t n) {
    uint16_t count;
    switch (bpp) {
        case 1:
            count = n >> 3;
            while (n) {
                uint8_t tmp = 0;
                for (uint8_t bit = 0; bit < 8; bit++) {
                    tmp <<= 1; // TODO: this causes extra shift of 0, but left-justifies correctly
                    if (n) {
                        tmp |= (*in++) & 1;
                        n--;
                    }
                }
                *out++ = tmp;
            }
        break;
        case 2:
            count = n >> 2;
            while (n) {
                uint8_t tmp = 0;
                for (uint8_t bit = 0; bit < 4; bit++) {
                    tmp <<= 2; // TODO: this causes extra shift of 0, but left-justifies correctly
                    if (n) {
                        tmp |= (*in++) & 3;
                        n--;
                    }

                }
                *out++ = tmp;
            }
        break;
        case 4:
            count = n >> 1;
            while (n) {
                uint8_t tmp = 0;
                for (uint8_t pixel = 0; pixel < 2; pixel++) {
                    tmp <<= 4; // TODO: this causes extra shift of 0, but left-justifies correctly
                    if (n) {
                        tmp |= (*in++) & 0x0f;
                        n--;
                    }
                }
                *out++ = tmp;
            }
        break;
    }
    return count;
}

void setGraphicsBase(uint32_t base) {
    gfxBase = base;
    *(uint16_t*) VERT_OFFSET = (uint16_t) (base >> 3);
}

uint16_t getWidth() {
    return widthPixels;
}

uint16_t getHeight() {
    return heightPixels;
}

uint16_t getBytesPerRow() {
    return bytesPerRow;
}

uint16_t getFrameSize() {
    return bufferSizeBytes;
}
