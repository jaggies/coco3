/*
 * sphere.h
 *
 *  Created on: Dec 9, 2018
 *      Author: jmiller
 */

#ifndef APPS_INTRAY_ISPHERE_H_
#define APPS_INTRAY_ISPHERE_H_

#include "iray.h"
#include "iphong.h"

typedef struct _iSphere {
    iPhong* shader; // This must be first
    Vec3i center;
    fixed r2; // radius squared
} iSphere;

extern iSphere* sp_create(fixed x, fixed y, fixed z, fixed radius, iPhong* shader);
extern int sp_isect(iSphere* sp, iRay* ray, fixed* tmax);
extern void sp_normal(iSphere* sp, iRay* ray, fixed t, Vec3i* outPoint, Vec3i* outNormal);
extern void sp_print(iSphere* sp);

#endif /* APPS_INTRAY_ISPHERE_H_ */
