/*
 * fixed.c
 *
 *  Created on: Dec 9, 2018
 *      Author: jmiller
 */

#include "fixed.h"

long mask = ((1 << fraction) - 1);

const fixed c_zero = 0;
const fixed c_half = 1 << (fraction - 1);
const fixed c_one = 1 << fraction;
const fixed c_two = 2 << fraction;
const fixed c_epsilon = 1 << (fraction  - 7); 
#ifdef COCO
const fixed c_pi = 3 << fraction; // Meh, close enough. CMOC can't do static calls to functions
#else
const fixed c_pi = 3.14159f * (1 << fraction);
#endif

float toFloat(fixed value) {
    return (float) value / (1 << fraction);
}

// multiplication with rounding
fixed fmult(fixed a, fixed b) {
    fresult x = a;
    fresult y = b;
    return (fixed) ((x * y + (1 << (fraction-1))) >> fraction);
}

fixed fdiv(fixed a, fixed b) {
    fresult r = ((fresult) a) << fraction;
    return (fixed) (r / b);
}

fixed toFixed(float value) {
    return (fixed) (value * (1 << fraction));
}

