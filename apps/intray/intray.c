/*
 * intray.c
 *
 *  Created on: Dec 9, 2018
 *      Author: jmiller
 */

#include <stdio.h>
#include "isphere.h"
#include "icamera.h"
#include "imath.h"

const int width = 80;
const int height = 40;

int main(int argc, char** argv) {
    Vec3i from;
    Vec3i at;
    Vec3i up;
    iCamera cam;
    iSphere sp;

    /* Create the camera */
    fixed eye = toFixed(2.0f);
    ivec3(eye, eye, eye, &from);
    ivec3(c_zero, c_zero, c_zero, &at);
    ivec3(c_zero, c_one, c_zero, &up);
    cam_create(&from, &at, &up, toFixed(45.0f), toFixed(1.0f), &cam);

    /* Create the scene */
    sp_create(c_zero, c_zero, c_zero, toFixed(0.25f), &sp);
    //sp_print(&sp);

    fixed u = 0, v = 0;
    fixed du = toFixed(1.0f) / (width-1);
    fixed dv = toFixed(1.0f) / (height-1);
    for (int j = 0; j < height; j++, v += dv) {
        u = 0;
        for (int i = 0; i < width; i++, u += du) {
            iRay ray;
            fixed tmax = toFixed(31.0f);
            cam_makeRay(&cam, u, v, &ray);
//            printf("Ray: %f %f %f - %f %f %f\n",
//                    toFloat(ray.point.x), toFloat(ray.point.y), toFloat(ray.point.z),
//                    toFloat(ray.dir.x), toFloat(ray.dir.y), toFloat(ray.dir.z));
            if (sp_isect(&sp, &ray, &tmax)) {
                putchar('*');
            } else {
                putchar('.');
            }
        }
        putchar('\n');
    }
    return 0;
}
