/*
 * cc3util.c
 *
 *  Created on: Mar 1, 2019
 *      Author: jmiller
 */
#include "os.h"
#include "fixed.h"
#include "ivec3i.h"
#include "dither.h"
#include "cc3gfx.h"
#include "cc3util.h"

#define DACBITS 2 // number of bits per channel on the CoCo3

// These work together with makeRgbPalette() and ditherVec3i()
static uint8_t redBits = 1;
static uint8_t grnBits = 1;
static uint8_t bluBits = 1;
static uint8_t redShift = 0;
static uint8_t grnShift = 0;
static uint8_t blushift = 0;

// HMM. These should have unsigned versions too..
uint16_t min(uint16_t a, uint16_t b) {
    return a < b ? a : b;
}

uint16_t max(uint16_t a, uint16_t b) {
    return a > b ? a : b;
}

int16_t abs(int16_t v) {
    return v >= 0 ? v : -v;
}

void makeRgbPalette(uint8_t rbits, uint8_t gbits, uint8_t bbits) {
    uint8_t sr = DACBITS - (redBits = rbits);
    uint8_t sg = DACBITS - (grnBits = gbits);
    uint8_t sb = DACBITS - (bluBits = bbits);
    redShift = fraction - (redBits + 4); //
    grnShift = fraction - (grnBits + 4);
    blushift = fraction - (bluBits + 4);
    for (uint8_t i = 0; i < 16; i++) {
        uint8_t r = i >> (gbits + bbits);
        uint8_t g = (i >> bbits) & (((uint8_t)1 << gbits) - 1);
        uint8_t b = i & (((uint8_t)1 << bbits) - 1);
        setPalette(i, r << sr, g << sg, b << sb);
    }
}

uint8_t ditherVec3i(uint8_t i, uint8_t j, Vec3i* color) {
    uint8_t red = (uint8_t) (color->x >> redShift);
    red = dither(redBits+4, redBits, (uint8_t) i, (uint8_t) j, red);
    uint8_t grn = (uint8_t) (color->y >> grnShift);
    grn = dither(grnBits+4, grnBits, (uint8_t) i, (uint8_t) j, grn);
    uint8_t blu = (uint8_t) (color->z >> blushift);
    blu = dither(bluBits+4, bluBits, (uint8_t) i, (uint8_t) j, blu);
    return (red << (bluBits + bluBits)) | (grn << bluBits) | blu;
}



