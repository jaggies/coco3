/*
 * light.h
 *
 *  Created on: Dec 23, 2018
 *      Author: jmiller
 */

#ifndef LIB_RAY_LIGHT_H_
#define LIB_RAY_LIGHT_H_

#include "ivec3i.h"

// A simple directional light
typedef struct _Light {
    Vec3i direction; // ray direction from light source
    Vec3i color;
} iLight;

iLight* light_create(Vec3i* dir, Vec3i* color);

#endif /* LIB_RAY_LIGHT_H_ */
