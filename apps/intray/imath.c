/*
 * imath.c
 *
 *  Created on: Dec 9, 2018
 *      Author: jmiller
 */

#include "imath.h"

// Quick integer square rooter
long isqrt(long value) {
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
    return fmult(toFixed(22.627416998f), isqrt(value));
}

