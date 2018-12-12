/*
 * imath.c
 *
 *  Created on: Dec 9, 2018
 *      Author: jmiller
 */

#include "imath.h"

extern double sqrt(double);

// Quick integer square rooter
fresult isqrt(fresult value) {
    long result = 0;
    long bit = 1 << (8 * sizeof(fixed) - 2); // The second-to-top bit is set: 1 << 30 for 32 bits
    while (bit > value) {
        // "bit" starts at the highest power of four <= the argument.
        bit >>= 2;
    }
    while (bit != 0) {
        if (value >= result + bit) {
            value -= result + bit;
            result += bit << 1;
        }
        result >>= 1;
        bit >>= 2;
    }
    return result;
}

fixed fsqrt(fixed value) {
    // approximation: isqrt(a<<fraction) ~= a*sqrt(512)
    //const fixed sqrtFrac = toFixed(22.627416998f); // sqrt512
    const fixed sqrtFrac = toFixed(sqrt(1 << fraction));
    return fmult(sqrtFrac, isqrt(value)); // more accurate: isqrt(value << fraction);
}

