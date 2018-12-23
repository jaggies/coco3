/*
 * dither.c
 *
 *  Created on: Dec 12, 2018
 *      Author: jmiller
 */

#include "dither.h"

static const int dmat[4][4] = {
          { 0, 12,  3, 15},
          { 8,  4, 11,  7},
          { 2, 14,  1, 13},
          {10,  6,  9,  5} };

const int dithBits = 4; // number of effective levels from dithering

int dither(int inBits, int outBits, int x, int y, int grey) {
     int thresh, val, err, n;
     // the threshold for the decision
     thresh = dmat[x&3][y&3];

     // Lower of the two possible values, due to integer division
     val = grey >> (inBits - outBits);

     // Error for choosing this value
     err = grey - (val << (inBits - outBits));

     // Calculate normalized value between 0 and 15 for given error
     n = err >> (inBits - outBits - dithBits);

    return (n > thresh) ? (val+1) : val;
 }


