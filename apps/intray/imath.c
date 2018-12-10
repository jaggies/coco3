/*
 * imath.c
 *
 *  Created on: Dec 9, 2018
 *      Author: jmiller
 */

#include "imath.h"

fixed isqrt(fixed value) {
    fixed a, b;
    if (value < 2) return value;
    a = 1255;
    b = value / a;
    a = (a + b) / 2;
    b = value / a; a = (a + b) / 2;
    b = value / a; a = (a + b) / 2;
    b = value / a; a = (a + b) / 2;
    return a;
}



