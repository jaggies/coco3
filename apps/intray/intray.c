/*
 * intray.c
 *
 *  Created on: Dec 9, 2018
 *      Author: jmiller
 */

#include "os.h"
#include "isphere.h"
#include "icamera.h"
#include "imath.h"

const int width = 40;
const int height = 20;

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
    Vec3i lightdir;
    ivec3(toFixed(1), toFixed(1), toFixed(1), &lightdir);
    inormalize3(&lightdir);
    printf("Light dir: ");
    ivec3_print(&lightdir);
    for (int j = 0; j < height; j++, v += dv) {
        u = 0;
        for (int i = 0; i < width; i++, u += du) {
            iRay ray;
            fixed tmax = toFixed(31.0f);
            cam_makeRay(&cam, u, v, &ray);
            if (sp_isect(&sp, &ray, &tmax)) {
                Vec3i normal;
                sp_normal(&sp, &ray, tmax, &normal);
                fixed d = idot3(&lightdir, &normal);
                putchar('0' + (d >> (fraction - 3)));
            } else {
                putchar('.');
            }
        }
        putchar('\n');
    }
    return 0;
}
