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

float toFloat(fixed value);
fixed mult(fixed a, fixed b);
fixed toFixed(float value);

extern fixed c_one;
extern fixed c_two;
extern fixed c_pi;
extern long mask;

#endif /* APPS_MINRAY_FIXED_H_ */
