/*
 * icamera.c
 *
 *  Created on: Dec 9, 2018
 *      Author: jmiller
 */

#include "os.h"
#include "icamera.h"

void cam_makeRay(iCamera* camera, fixed u, fixed v, iRay* ray) {
    icopy3(&camera->baseray.point, &ray->point);
    iaddscaled3(&camera->baseray.dir, u, &camera->du, &ray->dir);
    iaddscaled3(&ray->dir, v, &camera->dv, &ray->dir);
    inormalize3(&ray->dir);
}

void cam_create(Vec3i* from, Vec3i* at, Vec3i* up, fixed fov, fixed aspect, iCamera* camera) {
    // 2.0f * tanf(Radians(fov / 2.0f));
    fixed radians = fmult(c_pi, fov / 360);
    fixed tanfov2 = 2*ftan(radians);
    Vec3i dir, upNormalized, tdu, tdv;
    isub3(at, from, &dir);
    inormalize3(&dir);
    icopy3(up, &upNormalized); inormalize3(&upNormalized);
    icross(&dir, &upNormalized, &tdu); inormalize3(&tdu); imult3s(&tdu, tanfov2, &tdu);
    icross(&tdu, &dir, &tdv); inormalize3(&tdv); imult3s(&tdv, tanfov2, &tdv);
    imult3s(&tdu, aspect, &tdu);
    icopy3(&tdu, &camera->du);
    icopy3(&tdv, &camera->dv);
    icopy3(from, &camera->baseray.point);

	// camera->dir = dir - 0.5*du - 0.5*dv
    iaddscaled3(&dir, -c_half, &camera->du, &camera->baseray.dir);
    iaddscaled3(&camera->baseray.dir, -c_half, &camera->dv, &camera->baseray.dir);
}


