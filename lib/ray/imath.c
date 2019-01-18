/*
 * imath.c
 *
 *  Created on: Dec 9, 2018
 *      Author: jmiller
 */
#include "os.h"
#include "imath.h"

// Quick integer square rooter
fresult isqrt(fresult value) {
    fresult bit = (fresult) 1 << (8 * sizeof(fresult) - 2); // highest non-sign bit
    if (value < 0) {
        printf("Exception: sqrt(%d)\n", (int) value);
        return (fresult) 0;
    }
    while (bit > value) {
        bit >>= 2;
    }
    fresult result = 0;
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
    return (fixed) isqrt((fresult) value << fraction);
}

fixed power(fixed x, int8_t y)
{
    assert(y >= 0);
    if (y == 0) return c_one;
    fixed z = power(x, y >> 1);
    z = fmult(z,z);
    return ((y&1) == 0) ? z : fmult(x, z);
}

fixed fpow(fixed x, fixed y) {
    return power(x, toInt8(y));
}
