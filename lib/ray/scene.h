/*
 * scene.h
 *
 *  Created on: Dec 24, 2018
 *      Author: jmiller
 */

#ifndef LIB_RAY_SCENE_H_
#define LIB_RAY_SCENE_H_

#include "icamera.h"
#include "isphere.h"
#include "itriangle.h"
#include "ilight.h"

typedef struct _Scene {
    iCamera* camera;
    iSphere* spheres;
    iTriangle* triangles;
    iLight* lights;
    int nSphere;
    int nTriangle;
    int nLight;
    Vec3i background;
} Scene;

#endif /* LIB_RAY_SCENE_H_ */
