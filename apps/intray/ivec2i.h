/*
 * imath.h
 *
 *  Created on: Dec 9, 2018
 *      Author: jmiller
 */

#ifndef APPS_MINRAY_IMATH_H_
#define APPS_MINRAY_IMATH_H_

#include "fixed.h"

typedef struct Vec2i {
    fixed x, y;
} Vec2i;

// Vec2i operations
extern void ivec2(fixed x, fixed y, Vec2i* result);
extern void iadd2(Vec2i* a, Vec2i* b, Vec2i* result);
extern void iaddscaled2(Vec2i* a, fixed s, Vec2i* b, Vec2i* result);
extern void isub2(Vec2i* a, Vec2i* b, Vec2i* result);
extern void imult2(Vec2i* a, fixed t, Vec2i* result);
extern fixed idot2(Vec2i* a, Vec2i* b);
extern void inormalize2(Vec2i* n);
extern void icopy2(Vec2i* a, Vec2i* result);
extern void inegate2(Vec2i* a);
extern void imultadd2(Vec2i* a, Vec2i* b, Vec2i* c, Vec2i* result);
extern fixed isqrt(fixed a);

#endif /* APPS_MINRAY_IMATH_H_ */
