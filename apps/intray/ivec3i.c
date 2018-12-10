/*
 * ivec3i.c
 *
 *  Created on: Dec 9, 2018
 *      Author: jmiller
 */

/*
 * imath.c
 *
 *  Created on: Dec 9, 2018
 *      Author: jmiller
 */

#include "assert.h"
#include "imath.h"
#include "ivec3i.h"

void ivec3(fixed x, fixed y, fixed z, Vec3i* result) {
    result->x = x;
    result->y = y;
    result->z = z;
}

void iadd3(Vec3i* a, Vec3i* b, Vec3i* result) {
    result->x = a->x + b->x;
    result->y = a->y + b->y;
    result->z = a->z + b->z;
}

void iaddscaled3(Vec3i* a, fixed s, Vec3i* b, Vec3i* result) {
    result->x = a->x + fmult(s, b->x);
    result->y = a->y + fmult(s, b->y);
    result->z = a->z + fmult(s, b->z);
}

void isub3(Vec3i* a, Vec3i* b, Vec3i* result) {
    result->x = a->x - b->x;
    result->y = a->y - b->y;
    result->z = a->z - b->z;
}

void imult3(Vec3i* a, fixed t, Vec3i* result) {
    result->x = fmult(a->x, t);
    result->y = fmult(a->y, t);
    result->z = fmult(a->z, t);
}

void icross(Vec3i* a, Vec3i* b, Vec3i* result) {
    result->x = fmult(a->y, b->z) - fmult(a->z, b->y);
    result->y = fmult(a->z, b->x) - fmult(a->x, b->z);
    result->z = fmult(a->x, b->y) - fmult(a->y, b->x);
}

fixed idot3(Vec3i* a, Vec3i* b) {
    return fmult(a->x, b->x) + fmult(a->y, b->y) + fmult(a->z, b->z);
}

void inormalize3(Vec3i* n) {
    fixed len2 = idot3(n, n);
    fixed len = fsqrt(len2);
    assert(len != 0);
    fixed lenInv = fdiv(c_one, len);
    imult3(n, lenInv, n);
}

void icopy3(Vec3i* a, Vec3i* result) {
    result->x = a->x;
    result->y = a->y;
    result->z = a->z;
}

void inegate3(Vec3i* a) {
    a->x = -a->x;
    a->y = -a->y;
    a->z = -a->z;
}

void imultadd3(Vec3i* a, Vec3i* b, Vec3i* c, Vec3i* result) {
    result->x = fmult(a->x, b->x) + c->x;
    result->y = fmult(a->y, b->y) + c->y;
    result->z = fmult(a->z, b->z) + c->z;
}

void ivec3_print(Vec3i* v) {
    printf("<%f, %f, %f>\n", toFloat(v->x), toFloat(v->y), toFloat(v->z));
}

