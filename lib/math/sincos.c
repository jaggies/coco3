/*
 * sin.c
 *
 *  Created on: Apr 2, 2019
 *      Author: jmiller
 */

#include <math.h>

// Chebychev approximation good for ~3 digits
float _cos(float x) {
    const float c1 = 0.99940307f;
    const float c2 = -0.49558072f;
    const float c3 = 0.03679168f;

    float x2 = x * x;
    return (c1 + x2 * (c2 + c3 * x2));
}

float cos(float x) {
    const float pi = 3.14159265359f;
    const float pi2 = 6.28318530718f;
    const float two_over_pi = 0.63661977236f;

    while (x > pi2) {
        x -= pi2;
    }
    x = (x < 0) ? -x : x; // cos(-x) = cos(x)
    switch ((int) (x * two_over_pi)) {
        case 0: return _cos(x);
        case 1: return - _cos(pi - x);
        case 2: return - _cos(x - pi);
        case 3: return _cos(pi2 - x);
        default: return 0.0f; // oops
    }
}

float sin(float x) {
    const float m_pi2 = 1.57079632679f;
    return cos(m_pi2 - x);
}
