/*
 * ivec3i.c
 *
 *  Created on: Dec 9, 2018
 *      Author: jmiller
 */

#include "os.h"
#include "fmath.h"
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

void imult3s(Vec3i* a, fixed t, Vec3i* result) {
    result->x = fmult(a->x, t);
    result->y = fmult(a->y, t);
    result->z = fmult(a->z, t);
}

void imult3(Vec3i* a, Vec3i* b, Vec3i* result) {
    result->x = fmult(a->x, b->x);
    result->y = fmult(a->y, b->y);
    result->z = fmult(a->z, b->z);
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
    if (len2 <= 0) { // overflow or degenerate normal
        #ifdef DEBUG
        printf("OOPS! len2 = %x vec=", len2);
        ivec3_print(n);
        #endif
        return;
    }
    fixed len = fsqrt(len2);
    fixed lenInv = fdiv(c_one, len);
    imult3s(n, lenInv, n);
}

void icopy3(Vec3i* a, Vec3i* result) {
	*result = *a;
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

void ireflect3(Vec3i* incident, Vec3i* normal, Vec3i* result)
{
    fixed scale = fmult(c_two, idot3(incident, normal));
    imult3s(normal, scale, result);
    isub3(incident, result, result);
}

int itransmit3(fixed eta1, fixed eta2, Vec3i* incident, Vec3i* normal, Vec3i* result)
{
    // Heckbert's method (T = eta*I + (eta * costheta1 - sqrt(costheta2sq)) * N)
    fixed eta = fdiv(eta1, eta2);
    fixed costheta1 = -idot3(incident, normal);
    fixed costheta2sq = c_one - fmult(fmult(eta,eta), c_one - fmult(costheta1, costheta1));
    if (costheta2sq >= 0) {
        Vec3i tmp;
        imult3s(incident, eta, result);
        imult3s(normal, fmult(eta, costheta1) - fsqrt(costheta2sq), &tmp);
        iadd3(&tmp, result, result);
        return 1;
    }
    return 0; // total internal reflection
}

