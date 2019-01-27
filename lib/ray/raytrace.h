/*
 * trace.h
 *
 *  Created on: Dec 23, 2018
 *      Author: jmiller
 */

#ifndef LIB_RAY_RAYTRACE_H_
#define LIB_RAY_RAYTRACE_H_

#include "iray.h"
#include "ihit.h"
#include "scene.h"

int trace(Scene* scene, iRay* ray, iHit* hit);
void shade(Scene* scene, iRay* ray, iHit* hit, Vec3i* color, uint8_t depth);

#endif /* LIB_RAY_RAYTRACE_H_ */
