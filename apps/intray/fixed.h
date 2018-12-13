/*
 * fixed.h
 *
 *  Created on: Dec 9, 2018
 *      Author: jmiller
 */

#ifndef APPS_MINRAY_FIXED_H_
#define APPS_MINRAY_FIXED_H_

#ifdef COCO
typedef int Halfword;
typedef long Word; // intermediate result (e.g. multiply/divide)
#define fraction 9
#else
#include <stdint.h>
typedef int16_t Halfword;
typedef int32_t Word; // intermediate result (e.g. multiply/divide)
#define fraction 9
#endif

typedef Halfword fixed;
typedef Word fresult;

extern float toFloat(fixed value);
extern fixed toFixed(float value);

extern fixed fmult(fixed a, fixed b);
extern fixed fdiv(fixed a, fixed b);
extern fixed fsqrt(fixed value);

extern const fixed c_zero;
extern const fixed c_one;
extern const fixed c_two;
extern const fixed c_pi;
extern const fixed c_half;
extern const fixed c_epsilon;

extern long mask;

#endif /* APPS_MINRAY_FIXED_H_ */
