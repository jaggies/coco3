/*
 * fixed.h
 *
 *  Created on: Dec 9, 2018
 *      Author: jmiller
 */

#ifndef LIB_MATH_FIXED_H_
#define LIB_MATH_FIXED_H_

#ifdef COCO
typedef int Halfword;
typedef long Word; // intermediate result (e.g. multiply/divide)
#define fraction 9
#else
#include <stdint.h>
typedef int32_t Halfword;
typedef int64_t Word; // intermediate result (e.g. multiply/divide)
#define fraction 24
#endif

typedef Halfword fixed;
typedef Word fresult;

extern float toFloat(fixed value);
extern int8_t toInt8(fixed value);
extern fixed fromFloat(float value);
extern fixed fromInt(int value);
extern fixed clamp(fixed value, fixed min, fixed max);
extern fixed fmult(fixed a, fixed b);
extern fixed fdiv(fixed a, fixed b);
extern fixed fsqrt(fixed value);
extern fixed ftan(fixed value);
extern fixed fpow(fixed x, fixed y);

extern const fixed c_zero;
extern const fixed c_one;
extern const fixed c_two;
extern const fixed c_pi;
extern const fixed c_half;
extern const fixed c_epsilon;
extern const fixed c_max; // maximum value

extern long mask;

#endif /* LIB_MATH_FIXED_H_ */
