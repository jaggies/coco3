/*
 * fixed.c
 *
 *  Created on: Dec 9, 2018
 *      Author: jmiller
 */

#include "fixed.h"

long mask = ((1 << fraction) - 1);

fixed c_zero = 0;
fixed c_half = 1 << (fraction - 1);
fixed c_one = 1 << fraction;
fixed c_two = 2 << fraction;
fixed c_pi = 3 << fraction; // Meh, close enough. CMOC can't do static calls to functions

float toFloat(fixed value) {
    return (float) (value >> fraction) + (float)(value & mask) / mask;
}

fixed fmult(fixed a, fixed b) {
    mulresult x = a;
    mulresult y = b;
    return (fixed) (x * y >> fraction);
}

fixed fdiv(fixed a, fixed b) {
    mulresult r = a << fraction;
    return r / b;
}

fixed toFixed(float value) {
    return (fixed) (value * (1 << fraction));
}

