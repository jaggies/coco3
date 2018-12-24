/*
 * raytrace.h
 *
 *  Created on: Dec 23, 2018
 *      Author: jmiller
 */

#ifndef LIB_RAY_RAYTRACE_H_
#define LIB_RAY_RAYTRACE_H_

#include "iray.h"
#include "hit.h"
#include "scene.h"

int raytrace(Scene* scene, iRay* ray, Hit* hit);
void shade(Scene* scene, iRay* ray, Hit* hit, Vec3i* color);

#endif /* LIB_RAY_RAYTRACE_H_ */
