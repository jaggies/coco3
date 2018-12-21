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

const int WIDTH = 79;
const int HEIGHT = 24;

void runTests() {
    for (int i = 0; i < 64; i++) {
        printf("sqrt(%f) = %f\n", toFloat(fromInt(i)), toFloat(fsqrt(fromInt(i))));
    }
}

int main(int argc, char** argv) {
    Vec3i from;
    Vec3i at;
    Vec3i up;
    iCamera cam;
    iSphere sp;

    /* Create the camera */
    ivec3(c_one, c_zero, c_zero, &from);
    ivec3(c_zero, c_zero, c_zero, &at);
    ivec3(c_zero, c_one, c_zero, &up);
    cam_create(&from, &at, &up, fromInt(45), fromInt(1), &cam);

    /* Create light */
    Vec3i lightdir;
    ivec3(c_zero, -c_one, c_zero, &lightdir);
    inormalize3(&lightdir);

    /* Create the scene */
    sp_create(c_zero, c_zero, c_zero, fromFloat(0.35f), &sp);

    printf("from: "); ivec3_print(&from);
    printf("at: "); ivec3_print(&at);
    printf("up: "); ivec3_print(&up);
    printf("lightdir: "); ivec3_print(&lightdir);

    for (int j = 0; j < HEIGHT; j++) {
        fixed v = (fixed) ((fresult) c_one * j / HEIGHT);
        for (int i = 0; i < WIDTH; i++) {
            fixed tmax = c_max;
            fixed u = (fixed) ((fresult) c_one * i / WIDTH);
            iRay ray;
            cam_makeRay(&cam, u, v, &ray);
            if (sp_isect(&sp, &ray, &tmax)) {
                Vec3i normal;
                sp_normal(&sp, &ray, tmax, &normal);
                fixed d = idot3(&lightdir, &normal);
                putchar(d > 0 ? (char) ('A' + (d >> (fraction - 5))) : '.');
            } else {
                putchar('.');
            }
        }
        putchar('\n');
    }
    return 0;
}
