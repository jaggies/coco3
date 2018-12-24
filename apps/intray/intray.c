/*
 * intray.c
 *
 *  Created on: Dec 9, 2018
 *      Author: jmiller
 */

#include "os.h"
#include "scene.h"
#include "hit.h"
#include "imath.h"
#include "raytrace.h"
#include "testscene.h"

const int WIDTH = 79;
const int HEIGHT = 35;

void runTests() {
    for (int i = 0; i < 64; i++) {
        printf("sqrt(%f) = %f\n", toFloat(fromInt(i)), toFloat(fsqrt(fromInt(i))));
    }
}

int main(int argc, char** argv) {
    /* Create the scene */
    Scene *scene = testScene();

    for (int j = 0; j < HEIGHT; j++) {
        fixed v = (fixed) ((fresult) c_one * j / HEIGHT);
        for (int i = 0; i < WIDTH; i++) {
            iRay ray;
            Vec3i color;
            Hit hit;
            fixed u = (fixed) ((fresult) c_one * i / WIDTH);
            cam_makeRay(scene->camera, u, v, &ray);
            hit.t = c_max;
            hit.object = 0;
            ivec3(0,0,0, &color);
            if (raytrace(scene, &ray, &hit)) {
                shade(scene, &ray, &hit, &color);
                uint8_t red = (uint8_t) (color.x >> (fraction + 1 - 8)); /* 0..255 */
                uint8_t grn = (uint8_t) (color.y >> (fraction + 1 - 8)); /* 0..255 */
                uint8_t blu = (uint8_t) (color.z >> (fraction + 1 - 8)); /* 0..255 */
                putchar('.' + (red+grn+blu) / 30);
            } else {
                putchar('.');
            }
        }
        putchar('\n');
    }
    return 0;
}
