/*
 * cc3gfx.c
 *
 *  Created on: Feb 19, 2019
 *      Author: jmiller
 */

#include "os.h"
#include "cc3hw.h"
#include "cc3gfx.h"
#include "cc3raster.h"

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

GfxState gfx;

// Filled out to 8 entries so these can share one mask
static uint8_t masks1bpp[] = { 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01 };
static uint8_t masks2bpp[] = { 0xc0, 0x30, 0x0c, 0x03, 0xc0, 0x30, 0x0c, 0x03 };
static uint8_t masks4bpp[] = { 0xf0, 0x0f, 0xf0, 0x0f, 0xf0, 0x0f, 0xf0, 0x0f };

static void setPixels1bpp(uint16_t x, uint16_t y, uint8_t* clr, uint16_t n);
static void setPixels2bpp(uint16_t x, uint16_t y, uint8_t* clr, uint16_t n);
static void setPixels4bpp(uint16_t x, uint16_t y, uint8_t* clr, uint16_t n);

void (*setPixels)(uint16_t x, uint16_t y, uint8_t* clr, uint16_t n);

// Constructs a palette entry of RGBRGB in the hardware format for Coco3.
uint8_t toPalette(uint8_t r, uint8_t g, uint8_t b) {
    uint8_t result = 0;
    result |= (r & 2) << 4;
    result |= (g & 2) << 3;
    result |= (b & 2) << 2;
    result |= (r & 1) << 2;
    result |= (g & 1) << 1;
    result |= (b & 1);
    return result;
}

int setMode(uint16_t xres, uint16_t yres, uint8_t depth) {
    uint8_t vrr = 0; // video resolution register setting when done
    switch (depth) {
        case 1: vrr |= CRES_1BPP; gfx.masks = &masks1bpp[0]; setPixels = setPixels1bpp; break;
        case 2: vrr |= CRES_2BPP; gfx.masks = &masks2bpp[0]; setPixels = setPixels2bpp; break;
        case 4: vrr |= CRES_4BPP; gfx.masks = &masks4bpp[0]; setPixels = setPixels4bpp; break;
        default: return 0;
    }
    switch (xres) {
        case 160: gfx.width_pixels = 160; vrr |= (depth == 4 ? HRES6 : 0); break;
        case 256: gfx.width_pixels = 256; vrr |= (depth == 4 ? HRES6 : depth == 2 ? HRES4 : depth == 1 ? HRES2 : 0); break;
        case 320: gfx.width_pixels = 320; vrr |= (depth == 4 ? HRES7 : depth == 2 ? HRES5 : 0); break;
        case 640: gfx.width_pixels = 640; vrr |= (depth == 2 ? HRES7 : depth == 1 ? HRES5 : 0); break;
        default: return 0;
    }
    switch (yres) {
        case 192: gfx.height_pixels = 192; vrr |= LPF_192; break;
        case 200: gfx.height_pixels = 200; vrr |= LPF_200; break;
        case 225: gfx.height_pixels = 225; vrr |= LPF_225; break;
        default: return 0;
    }
    gfx.bpp = depth;
    gfx.bytes_per_row = (uint8_t) (xres * gfx.bpp >> 3);
    gfx.buffer_size_bytes = gfx.bytes_per_row * yres;
    *VIDEO_RES = vrr;
    *VIDEO_MODE = 0x80; // graphics mode, 60Hz
    *INIT0 = 0x4c; // Coco3 mode & MMU enabled
    *VERT_SCROLL = 0; // clear vertical scroll register
    setGraphicsDrawBase(0x60000L); // set to default ECB location
    setGraphicsViewBase(0x60000L);

    return 1;
}

void setPalette(uint8_t index, uint8_t r, uint8_t g, uint8_t b) {
    *(PALETTE_BASE+(index&0xf)) = toPalette(r, g, b);
}

// Pack color into a byte such that all pixels are set to the same color
uint8_t packColor(uint8_t color) {
    uint8_t pixels;
    switch (gfx.bpp) {
        case 1:
            pixels = (color & 1) ? 0xff : 0x00;
        break;
        case 2:
            pixels = color & 3;
            pixels |= pixels << 2;
            pixels |= pixels << 4;
        break;
        case 4:
            pixels = color & 0x0f;
            pixels |= pixels << 4;
        break;
    }
    return pixels;
}

void clear(uint8_t color) {
    memset24(gfx.base_addr, packColor(color), gfx.buffer_size_bytes);
}

void setPixel(uint16_t x, uint16_t y) {
    gfx.base_y_offset = y * gfx.bytes_per_row;
    gfx.rasterX = x;
    gfx.rasterY = y;
    gfx.pixel_mask = gfx.masks[(uint8_t) x & 7];
    rasterSet();
}

void setPixels1bpp(uint16_t x, uint16_t y, uint8_t* clr, uint16_t n) {
    assert((x & 3) == 0);
    memcpy24(gfx.base_addr + (x >> 3) + y * gfx.bytes_per_row, clr, n);
}

void setPixels2bpp(uint16_t x, uint16_t y, uint8_t* clr, uint16_t n) {
    assert((x & 3) == 0);
    memcpy24(gfx.base_addr + (x >> 2) + y * gfx.bytes_per_row, clr, n);
}

void setPixels4bpp(uint16_t x, uint16_t y, uint8_t* clr, uint16_t n) {
    assert((x & 1) == 0);
    memcpy24(gfx.base_addr + (x >> 1) + y * gfx.bytes_per_row, clr, n);
}

uint16_t packPixels(uint8_t* const in, uint8_t* out, uint16_t n) {
    uint16_t count;
    switch (gfx.bpp) {
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

void setGraphicsDrawBase(uint32_t base) {
    assert((base & 0x1fff) == 0); // must be on a page boundary
    gfx.base_addr = base;
    gfx.base_page = (uint8_t) (base >> PAGE_BITS) & (uint8_t) 0xff;
}

void setGraphicsViewBase(uint32_t base) {
    *(uint16_t*) VERT_OFFSET = (uint16_t) (base >> 3);
}

uint16_t getWidth() {
    return gfx.width_pixels;
}

uint16_t getHeight() {
    return gfx.height_pixels;
}

uint16_t getBytesPerRow() {
    return gfx.bytes_per_row;
}

uint16_t getFrameSize() {
    return gfx.buffer_size_bytes;
}
