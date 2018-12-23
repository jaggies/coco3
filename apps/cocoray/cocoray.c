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
#include "cocogfx.h"
#include "dither.h"

const int WIDTH = 400;
const int HEIGHT = 200;
const int COLORS = 32;
const char GFXMODE = HS640x192x4;
const int DPYWIDTH = 640;
const int DPYHEIGHT = 200;

// Enables 6309 Native mode for higher speed
void set6309Native() {
    asm {
        ldmd  #$01
    }
}

int main(int argc, char** argv) {
    Vec3i from;
    Vec3i at;
    Vec3i up;
    iCamera cam;
    iSphere sp;

    /* Speedups */
    set6309Native();
    initCoCoSupport();
    setHighSpeed(1);

    /* Graphics */
    hscreen(GFXMODE);
    for (uint8_t i = 0; i < 4; i++) {
        paletteRGB(i, i, i, i);
    }

    /* Create the camera */
    ivec3(c_zero, c_zero, c_one, &from);
    ivec3(c_zero, c_zero, c_zero, &at);
    ivec3(c_zero, c_one, c_zero, &up);
    cam_create(&from, &at, &up, fromInt(45), fromFloat(1.0f), &cam);

    /* Create the scene */
    sp_create(c_zero, c_zero, c_zero, fromFloat(0.35f), &sp);

    Vec3i lightdir;
    ivec3(c_one, -c_one, c_one, &lightdir);
    inormalize3(&lightdir);
    printf("Light dir: ");
    ivec3_print(&lightdir);
    fixed kdiff = c_one;
    for (int j = 0; j < HEIGHT; j++) {
        fixed v = (fixed) ((fresult) c_one * j / HEIGHT);
        for (int i = 0; i < WIDTH; i++) {
            iRay ray;
            fixed u = (fixed) ((fresult) c_one * i / WIDTH);
            fixed tmax = fromInt(31);
            hset(i, j, 3); // show which pixel we're working on
            cam_makeRay(&cam, u, v, &ray);
            uint8_t color = 0;
            if (sp_isect(&sp, &ray, &tmax)) {
                Vec3i normal;
                sp_normal(&sp, &ray, tmax, &normal);
                fixed d = idot3(&lightdir, &normal);
                if (d > 0) { // ignore backfaces
                    color = (uint8_t) (fmult(d, kdiff) >> ((1+fraction) - 8)); // 0..255
                    color = dither(8, 2, (uint8_t) i, (uint8_t) j, color);
                }
            }
            hset(i, j, color);
        }
    }
    while (1)
        ;
    return 0;
}
