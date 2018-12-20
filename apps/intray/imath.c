/*
 * imath.c
 *
 *  Created on: Dec 9, 2018
 *      Author: jmiller
 */
#include <stdio.h>
#include "imath.h"

// Quick integer square rooter
fresult isqrt(fresult value) {
    fresult bit = (fresult) 1 << (8 * sizeof(fresult) - 2); // highest non-sign bit
    if (value < 0) {
        printf("Exception: sqrt(%lld)\n", value);
        return 0;
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
    return isqrt(value << fraction);
}

