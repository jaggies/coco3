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
    result->x = a->x + mult(s, b->x);
    result->y = a->y + mult(s, b->y);
    result->z = a->z + mult(s, b->z);
}

void isub3(Vec3i* a, Vec3i* b, Vec3i* result) {
    result->x = a->x - b->x;
    result->y = a->y - b->y;
    result->z = a->z - b->z;
}

void imult3(Vec3i* a, fixed t, Vec3i* result) {
    result->x = mult(a->x, t);
    result->y = mult(a->y, t);
    result->z = mult(a->z, t);
}

fixed idot3(Vec3i* a, Vec3i* b) {
    return mult(a->x, b->x) + mult(a->y, b->y) + mult(a->z, b->z);
}

void inormalize3(Vec3i* n) {
    imult3(n, c_one / isqrt(idot3(n, n)), n);
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
    result->x = mult(a->x, b->x) + c->x;
    result->y = mult(a->y, b->y) + c->y;
    result->z = mult(a->z, b->z) + c->z;
}


