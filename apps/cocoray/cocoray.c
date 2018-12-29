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
#include "raytrace.h"
#include "testscene.h"

const int WIDTH = 400;
const int HEIGHT = 200;
const int COLORS = 32;
const char GFXMODE = HS640x192x4;
const int DPYWIDTH = 640;
const int DPYHEIGHT = 200;

#define RBITS 2
#define GBITS 2
#define BBITS 1

// Enables 6309 Native mode for higher speed
void set6309Native() {
    asm {
        ldmd  #$01
    }
}

int main(int argc, char** argv) {
    /* Speedups */
    set6309Native();
    initCoCoSupport();
    setHighSpeed(1);

    /* Graphics */
    hscreen(GFXMODE);
    for (uint8_t i = 0; i < 4; i++) {
        paletteRGB(i, i, i, i);
    }

    /* Create the scene */
    Scene *scene = testScene(WIDTH, HEIGHT);

    for (int j = 0; j < HEIGHT; j++) {
        fixed v = (fixed) ((fresult) c_one * j / HEIGHT);
        for (int i = 0; i < WIDTH; i++) {
            iRay ray;
            Vec3i color;
            Hit hit;
            fixed u = (fixed) ((fresult) c_one * i / WIDTH);
            hset(i, j, 3); // show which pixel we're working on
            cam_makeRay(scene->camera, u, v, &ray);
            hit.t = c_max;
            hit.object = 0;
            ivec3(0,0,0, &color);
            if (raytrace(scene, &ray, &hit)) {
                shade(scene, &ray, &hit, &color);
                uint8_t red = (uint8_t) (color.x >> (fraction + 1 - 8)); /* 0..255 */
                red = dither(8, RBITS, (uint8_t) i, (uint8_t) j, red);
                uint8_t grn = (uint8_t) (color.y >> (fraction + 1 - 8)); /* 0..255 */
                grn = dither(8, GBITS, (uint8_t) i, (uint8_t) j, grn);
                uint8_t blu = (uint8_t) (color.z >> (fraction + 1 - 8)); /* 0..255 */
                blu = dither(8, BBITS, (uint8_t) i, (uint8_t) j, blu);
                hset(i, j, red);
            } else {
                hset(i, j, 0);
            }
        }
    }

    while(1)
        ;
    return 0;
}
