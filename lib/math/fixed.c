/*
 * fixed.c
 *
 *  Created on: Dec 9, 2018
 *      Author: jmiller
 */

#include "os.h"
#include "cc3hw.h"
#include "fixed.h"

const fixed c_fraction_mask = ((1 << fraction) - 1);
const fixed c_zero = 0;
const fixed c_half = 1 << (fraction - 1);
const fixed c_one = 1 << fraction;
const fixed c_two = 2 << fraction;
const fixed c_epsilon = 1 << (fraction  - 7);
const fixed c_max = ((unsigned)1 << (8*sizeof(fixed)-1)) - 1;
#ifdef COCO
const fixed c_pi = 3 << fraction; // TODO. Close enough? CMOC can't do static calls to functions
#else
const fixed c_pi = 3.14159f * (1 << fraction);
#endif

float toFloat(fixed value) {
    return (float) value / c_one;
}

int8_t toInt8(fixed value) {
    return (int8_t) ((value + (1 << (fraction-1))) >> fraction);
}

fixed clamp(fixed value, fixed min, fixed max) {
    if (value > max) {
        return max;
    }
    if (value < min) {
        return min;
    }
    return value;
}

// multiplication with rounding
fixed fmult(fixed a, fixed b) {
#ifdef MULD_WAR // Oops. Emulators treat this as *unsigned* multiply :/
    return (fixed) (((int32_t)a * (int32_t)b + (int32_t)c_half) >> fraction);
#else
    return (fixed) ((fastmult(a,b) + c_half) >> fraction);
#endif
}

fixed fdiv(fixed a, fixed b) {
    fresult r = ((fresult) a) << fraction;
    return (fixed) (r / b);
}

fixed ftan(fixed x) {
    // tan(x) ~= x + x^3/3 + 2x^5/15
    return x + fmult(x, fmult(x, x))/3;
}

fixed fromFloat(float value) {
    return (fixed) (value * (1 << fraction));
}

fixed fromInt(int value) {
    //assert(abs(value) < (1 << (8*sizeof(fixed) - fraction - 1)));
    return value << fraction;
}

