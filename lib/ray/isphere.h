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
    Vec3i center;
    fixed r2; // radius squared
    iPhong* shader;
} iSphere;

extern void sp_create(fixed x, fixed y, fixed z, fixed radius, iPhong* shader, iSphere* sp);
extern int sp_isect(iSphere* sp, iRay* ray, fixed* tmax);
extern void sp_normal(iSphere* sp, iRay* ray, fixed t, Vec3i* normal);
extern void sp_print(iSphere* sp);

#endif /* APPS_INTRAY_ISPHERE_H_ */
