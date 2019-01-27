/*
 * itriangle.h
 *
 *  Created on: Jan 27, 2019
 *      Author: jmiller
 */

#ifndef LIB_RAY_ITRIANGLE_H_
#define LIB_RAY_ITRIANGLE_H_

#include "iray.h"
#include "iphong.h"

typedef struct _iTriangle {
    iPhong* shader; // This must be first
    Vec3i  point;
    Vec3i  normal;
    Vec3i  edge[2];
} iTriangle;

extern void tri_create(Vec3i* p0, Vec3i* p1, Vec3i* p2, iPhong* shader, iTriangle* sp);

extern int tri_isect(iTriangle* tri, iRay* ray, fixed* tmax);
extern void tri_normal(iTriangle* tri, iRay* ray, fixed t, Vec3i* outPoint, Vec3i* outNormal);
extern void tri_print(iTriangle* tri);

#endif /* LIB_RAY_ITRIANGLE_H_ */
