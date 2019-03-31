/*
 * dither.c
 *
 *  Created on: Dec 12, 2018
 *      Author: jmiller
 */

#include "os.h"
#include "dither.h"

static const uint8_t threshold[4][4] = {
          { 0, 12,  3, 15},
          { 8,  4, 11,  7},
          { 2, 14,  1, 13},
          {10,  6,  9,  5} };

static const uint8_t dithBits = 4; // number of effective levels from dithering

uint8_t dither(uint8_t inBits, uint8_t outBits, uint8_t x, uint8_t y, uint8_t grey) {
     // Lower of the two possible values, due to integer division
     uint8_t val = grey >> (inBits - outBits);

     // Error for choosing this value
     uint8_t err = grey - (val << (inBits - outBits));

     // Calculate normalized value between 0 and 15 for given error
     uint8_t t = err >> (inBits - outBits - dithBits);
     if (t > threshold[x&3][y&3]) {
         val++;
     }
     return val;
 }

// Dither 6 bits down to 2 bits
uint8_t dither6x2(uint8_t x, uint8_t y, uint8_t grey) {
    grey = (grey + (grey << 1)) >> 2; // = grey *3/4
    const uint8_t idx = ((y&3) << 2) | (x & 3);
    uint8_t result = grey >> 4;
    int d = ((uint8_t*)threshold)[idx];
    if ((grey & 0x0f) > d) {
        result++;
    }
    return result;
}


