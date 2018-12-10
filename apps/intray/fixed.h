/*
 * fixed.h
 *
 *  Created on: Dec 9, 2018
 *      Author: jmiller
 */

#ifndef APPS_MINRAY_FIXED_H_
#define APPS_MINRAY_FIXED_H_

typedef int fixed;
typedef long mulresult;

#define fraction 9

extern float toFloat(fixed value);
extern fixed toFixed(float value);

extern fixed fmult(fixed a, fixed b);
extern fixed fdiv(fixed a, fixed b);
extern fixed fsqrt(fixed value);

extern fixed c_zero;
extern fixed c_one;
extern fixed c_two;
extern fixed c_pi;
extern fixed c_half;

extern long mask;

#endif /* APPS_MINRAY_FIXED_H_ */
