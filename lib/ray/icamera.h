/*
 * icamera.h
 *
 *  Created on: Dec 9, 2018
 *      Author: jmiller
 */

#ifndef APPS_INTRAY_ICAMERA_H_
#define APPS_INTRAY_ICAMERA_H_

#include "iray.h"

typedef struct _iCamera {
    Vec3i du, dv; // orthogonal vectors define the camera plane
    iRay baseray; // ray originating at uv(0,0) pointing to lower left
} iCamera;

extern void cam_makeRay(iCamera* camera, fixed u, fixed v, iRay* ray);
extern void cam_create(Vec3i* from, Vec3i* at, Vec3i* up, fixed fov, fixed aspect, iCamera* camera);

#endif /* APPS_INTRAY_ICAMERA_H_ */
