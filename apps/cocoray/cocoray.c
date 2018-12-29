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

const int WIDTH = 320; // Render width / height
const int HEIGHT = 200;
const int COLORS = 32;
const char GFXMODE = HS320x192x16;
const int DPYWIDTH = 320;
const int DPYHEIGHT = 200;

#define RBITS 2
#define GBITS 1
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
    for (uint8_t i = 0; i < 16; i++) {
        uint8_t r = i >> (GBITS + BBITS);
        uint8_t g = (i >> BBITS) & ((1<<GBITS) - 1);
        uint8_t b = i & ((1 << BBITS) - 1);
        paletteRGB(i, r, g, b);
    }

    /* Create the scene */
    float aspect = (float) WIDTH / HEIGHT;
    aspect *= (float) DPYHEIGHT / DPYWIDTH;
    Scene *scene = testScene(fromFloat(aspect));

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
                uint8_t clr =  (red << (GBITS + BBITS)) | (grn << BBITS) | blu;
                hset(i, j, clr);
            } else {
                hset(i, j, 0);
            }
        }
    }

    while(1)
        ;
    return 0;
}
