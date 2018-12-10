/*
 * sphere.h
 *
 *  Created on: Dec 9, 2018
 *      Author: jmiller
 */

#ifndef APPS_INTRAY_SPHERE_H_
#define APPS_INTRAY_SPHERE_H_
#include "fixed.h"

struct _Sphere {
    Vec3i center;
    fixed r2; // radius squared
};

#endif /* APPS_INTRAY_SPHERE_H_ */
