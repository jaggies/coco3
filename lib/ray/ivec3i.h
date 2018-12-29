/*
 * ivec3i.h
 *
 *  Created on: Dec 9, 2018
 *      Author: jmiller
 */

#ifndef APPS_INTRAY_IVEC3I_H_
#define APPS_INTRAY_IVEC3I_H_

#include "fixed.h"

typedef struct Vec3i {
    fixed x, y, z;
} Vec3i;

// Vec3i operations
extern void ivec3(fixed x, fixed y, fixed z, Vec3i* result);
extern void iadd3(Vec3i* a, Vec3i* b, Vec3i* result);
extern void iaddscaled3(Vec3i* a, fixed s, Vec3i* b, Vec3i* result);
extern void isub3(Vec3i* a, Vec3i* b, Vec3i* result);
extern void imult3(Vec3i* a, Vec3i* b, Vec3i* result);
extern void imult3s(Vec3i* a, fixed t, Vec3i* result);
extern void icross(Vec3i* a, Vec3i* b, Vec3i* result);
extern fixed idot3(Vec3i* a, Vec3i* b);
extern void inormalize3(Vec3i* n);
extern void icopy3(Vec3i* a, Vec3i* result);
extern void inegate3(Vec3i* a);
extern void imultadd3(Vec3i* a, Vec3i* b, Vec3i* c, Vec3i* result);
extern void ireflect3(Vec3i* incident, Vec3i* normal, Vec3i* result);
extern int itransmit3(fixed eta1, fixed eta2, Vec3i* incident, Vec3i* normal, Vec3i* result);
extern void ivec3_print(Vec3i* v);

#endif /* APPS_INTRAY_IVEC3I_H_ */
