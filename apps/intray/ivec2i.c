/*
 * imath.c
 *
 *  Created on: Dec 9, 2018
 *      Author: jmiller
 */

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
    result->x = a->x + mult(s, b->x);
    result->y = a->y + mult(s, b->y);
}

void isub2(Vec2i* a, Vec2i* b, Vec2i* result) {
    result->x = a->x - b->x;
    result->y = a->y - b->y;
}

void imult2(Vec2i* a, fixed t, Vec2i* result) {
    result->x = mult(a->x, t);
    result->y = mult(a->y, t);
}

fixed idot2(Vec2i* a, Vec2i* b) {
    return mult(a->x, b->x) + mult(a->y, b->y);
}

void inormalize2(Vec2i* n) {
    imult2(n, c_one / isqrt(idot2(n, n)), n);
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
    result->x = mult(a->x, b->x) + c->x;
    result->y = mult(a->y, b->y) + c->y;
}

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
