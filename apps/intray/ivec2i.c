/*
 * imath.c
 *
 *  Created on: Dec 9, 2018
 *      Author: jmiller
 */

#include <stdio.h>
#include "imath.h"
#include "ivec2i.h"

void ivec2(fixed x, fixed y, Vec2i* result) {
    result->x = x;
    result->y = y;
}

void iadd2(Vec2i* a, Vec2i* b, Vec2i* result) {
    result->x = a->x + b->x;
    result->y = a->y + b->y;
}

void iaddscaled2(Vec2i* a, fixed s, Vec2i* b, Vec2i* result) {
    result->x = a->x + fmult(s, b->x);
    result->y = a->y + fmult(s, b->y);
}

void isub2(Vec2i* a, Vec2i* b, Vec2i* result) {
    result->x = a->x - b->x;
    result->y = a->y - b->y;
}

void imult2(Vec2i* a, fixed t, Vec2i* result) {
    result->x = fmult(a->x, t);
    result->y = fmult(a->y, t);
}

fixed idot2(Vec2i* a, Vec2i* b) {
    return fmult(a->x, b->x) + fmult(a->y, b->y);
}

void inormalize2(Vec2i* n) {
    imult2(n, c_one / fsqrt(idot2(n, n)), n);
}

void icopy2(Vec2i* a, Vec2i* result) {
    result->x = a->x;
    result->y = a->y;
}

void inegate2(Vec2i* a) {
    a->x = -a->x;
    a->y = -a->y;
}

void imultadd2(Vec2i* a, Vec2i* b, Vec2i* c, Vec2i* result) {
    result->x = fmult(a->x, b->x) + c->x;
    result->y = fmult(a->y, b->y) + c->y;
}

void ivec2_print(Vec2i* v) {
    printf("<%f, %f>\n", toFloat(v->x), toFloat(v->y));
}
