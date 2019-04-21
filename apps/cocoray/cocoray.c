/*
 * intray.c
 *
 *  Created on: Dec 9, 2018
 *      Author: jmiller
 */

#include "os.h"
#include "cc3hw.h"
#include "cc3gfx.h"
#include "cc3raster.h"
#include "isphere.h"
#include "icamera.h"
#include "imath.h"
#include "dither.h"
#include "raytrace.h"
#include "testscene.h"

const int WIDTH = 320; // Render width / height
const int HEIGHT = 225;
const int DPYWIDTH = 320;
const int DPYHEIGHT = 225;
const int DACBITS = 2;

#define MAXDEPTH 3 // depth for reflected rays

static Vec3i palette[16]; // 16-color table entries

void makePalette() {
    for (uint8_t i = 0; i < 16; i++) {
        fixed b = (i & 1) ? (c_one - 1) : 0;
        fixed g = ((i >> 1) & 3) * (c_one - 1) / 3;
        fixed r = ((i >> 3) & 1) ? (c_one - 1) : 0;
        ivec3(r, g, b, &palette[i]); // gray
    }
    for (uint8_t i = 0; i < 16; i++) {
        uint8_t red = (uint8_t) (palette[i].x >> (fraction - 2));
        uint8_t grn = (uint8_t) (palette[i].y >> (fraction - 2));
        uint8_t blu = (uint8_t) (palette[i].z >> (fraction - 2));
        setPalette(i, red, grn, blu);
    }
}

uint8_t nearest(Vec3i* color) {
    uint8_t index = 0;
    const uint8_t shift = (fraction + 1 - 8);
    fixed best = c_max;
    for (uint8_t i = 0; i < 16; i++) {
        Vec3i diff;
        isub3(color, &palette[i], &diff);
        fixed dist = idot3(&diff, &diff);
        if (dist < best) {
            index = i;
            best = dist;
        }
    }
    return index;
}

static Vec3i error;
uint8_t diffusion(Vec3i* color, int reset) {
    if (reset) {
        error.x = error.y = error.z = c_zero;
    }
    Vec3i adjColor;
    iadd3(&error, color, &adjColor);
    Vec3i clamped;
    clamped.x = clamp(adjColor.x, 0, c_one);
    clamped.y = clamp(adjColor.y, 0, c_one);
    clamped.z = clamp(adjColor.z, 0, c_one);

    uint8_t index = nearest(&clamped);
    Vec3i diff;
    isub3(color, &palette[index], &diff);
    iadd3(&error, &diff, &error);
    error.x = clamp(error.x, -c_one, c_one);
    error.y = clamp(error.y, -c_one, c_one);
    error.z = clamp(error.z, -c_one, c_one);
    return index;
}

int main(int argc, char** argv) {
    /* Speedups */
    set6309Native();
    initCoCoSupport();
    setHighSpeed(1);

    /* Graphics */
    setMode(DPYWIDTH, DPYHEIGHT, 4);
    makePalette();
    clear(0);

    /* Create the scene */
    float aspect = (float) WIDTH / HEIGHT;
    Scene *scene = testScene(fromFloat(aspect));

    for (int j = 0; j < HEIGHT; j++) {
        rasterPos(0, j);
        fixed v = c_one - (fixed) ((fresult) c_one * j / HEIGHT);
        for (int i = 0; i < WIDTH; i++) {
            iRay ray;
            Vec3i color;
            iHit hit;
            fixed u = (fixed) ((fresult) c_one * i / WIDTH);
            hset(i, j, 15); // show which pixel we're working on
            cam_makeRay(scene->camera, u, v, &ray);
            init_hit(&hit);
            ivec3(0,0,0, &color);
            if (trace(scene, &ray, &hit)) {
                shade(scene, &ray, &hit, &color, MAXDEPTH);
            } else {
                color = scene->background;
            }
            color.x = clamp(color.x, 0, c_one);
            color.y = clamp(color.y, 0, c_one);
            color.z = clamp(color.z, 0, c_one);
            rasterColor(diffusion(&color, i == 0));
            rasterSet();
            rasterIncX();
        }
    }

    while (1)
        ;

    return 0;
}
