/*
 * sphere.h
 *
 *  Created on: Dec 9, 2018
 *      Author: jmiller
 */

#ifndef APPS_INTRAY_ISPHERE_H_
#define APPS_INTRAY_ISPHERE_H_

#include "iray.h"

typedef struct _iSphere {
    Vec3i center;
    fixed r2; // radius squared
} iSphere;

extern void sp_create(fixed x, fixed y, fixed z, fixed radius, iSphere* sp);
extern int sp_isect(iSphere* sp, iRay* ray, fixed* tmax);
extern void sp_print(iSphere* sp);

#endif /* APPS_INTRAY_ISPHERE_H_ */
