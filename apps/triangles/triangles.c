/*
 * palette.c
 *
 *  Created on: Feb 12, 2019
 *      Author: jmiller
 */

#include "os.h"
#include "cc3hw.h"
#include "cc3gfx.h"
#include "cc3triangle.h"

#define WIDTH 320
#define HEIGHT 200
#define VBLANK (262 - HEIGHT)
#define DEPTH 4
#define SIZE 50

void simpleRGB() {
    for (uint8_t i = 0; i < 16; i++) {
        uint8_t red = (i & 1) ? 3 : 0;
        uint8_t grn = (i >> 1) & 3;
        uint8_t blu = ((i >> 3) & 1) ? 3 : 0;
        setPalette(i, red, grn, blu);
    }
}

uint8_t toColor(uint8_t r, uint8_t g, uint8_t b) {
    return (r >> 7) | ((g>>6) << 1) | ((b>>7) << 3);
}

static int myrand() {
    return rand() | (rand() >> 1);
}

typedef struct _Vertex {
    int16_t x;
    int16_t y;
} Vertex;

typedef struct _Color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} Color;

// Draws polygon test onto 640x480 raster
static Vertex p[][3] = {
        { { 0, 0 }, { 0, 0 }, { 0, 0 } },
        { { 320, 100 }, { 100, 300 }, { 540, 300 } },
        { { 320, 100 }, { 320, 0 }, { 600, 100 } },
        { { 320, 100 }, { 320, 0 }, { 20, 100 } },
        { { 320, 480 }, { 100, 300 }, { 540, 300 } },
        { { 320, 100 }, { 600, 100 }, { 540, 300 } },
        { { 320, 100 }, { 100, 300 }, { 20, 100 } },
        { { 320, 480 }, { 0, 400 }, { 640, 400 } }
};

// Per-vertex color array
static Color colr[][3] =  {
    { { 255, 0, 0 }, { 0, 220, 0 }, { 0, 0, 255 } },
    { { 255, 220, 255 }, { 150, 0, 2 }, { 100, 0, 0 } },
    { { 150, 0, 2 }, { 255, 220, 255 }, { 255, 220, 255 } }
};

void poly3(Vertex* p, uint8_t clr) {
    int16_t v[3][2];
    for (int8_t i = 0; i < 3; i++) {
        v[i][0] = (int32_t) p[i].x * WIDTH / 640; // scale data based on screen size
        v[i][1] = (int32_t) p[i].y * HEIGHT / 480;
    }
    triangle(&v[0][0], &v[1][0], &v[2][0], clr); // TODO: this assumes perfect int16 packing in Vertex
}

void testpoly() {
    uint8_t color[3];
    for (int8_t i = 0; i < 3; i++) {
        // Color array is supposed to be per-vertex. Just use the first color
        color[i] = toColor(colr[i][0].r, colr[i][0].g, colr[i][0].b);
    }

    const uint8_t order[] = { 1, 4, 5, 6, 7, 3, 2 };
    for (int8_t c = 0; c < 3; c++) {
        for (int8_t i = 0; i < 7; i++) {
            poly3(p[order[i]], color[c]);
        }
    }
//    float alpha = 0.0, theta = M_PI / 180, ang;
//    float phi = 120.0 * M_PI / 180.0;
//    while (1) {
//        float s = 0.0;
//        s = SZ * alpha / (2.0 * M_PI);
//        for (i = 0, ang = 0.0; i < 3; i++, ang += phi) {
//            p[i][X] = 320.0 + s * cos(alpha + ang);
//            p[i][Y] = 226.0 + s * sin(alpha + ang);
//            alpha += theta;
//        }
//        gcl_shademode(1);
//        poly3(p, colr);
//        gcl_shademode(0);
//        gcl_rgbcolor(0, 0, 0);
//        poly3(p, colr);
//    }
}

#define TRI_SIZE 100 // pixels
enum { X=0, Y};

void testCases() {
    Vertex verts[] = { { TRI_SIZE/2, 0}, { 0, TRI_SIZE }, { TRI_SIZE, TRI_SIZE } };
    int8_t cases[][3] = { {0, 1, 2}, {0, 2, 1}, {1, 0, 2}, {1, 2, 0}, {2, 0, 1}, {2, 1, 0} };
    for (uint8_t i = 0; i < 6; i++) {
        int16_t v0[2], v1[2], v2[2];
        v0[X] = verts[cases[i][0]].x;
        v1[X] = verts[cases[i][1]].x;
        v2[X] = verts[cases[i][2]].x;
        v0[Y] = verts[cases[i][0]].y;
        v1[Y] = verts[cases[i][1]].y;
        v2[Y] = verts[cases[i][2]].y;

        triangle(&v0, &v1, &v2, i);
    }
}

int main(int argc, char** argv) {
    /* Speedups */
    set6309Native();
    initCoCoSupport();
    setHighSpeed(1);

    /* Graphics */
    setMode(WIDTH, HEIGHT, DEPTH);
    simpleRGB();
    clear(0x8); // blue

    /* Draw pixels */
    const int height = getHeight();
    const int width = getWidth();

    testCases();

    sleep(100);

    testpoly();

    sleep(100);

    int16_t v[3][2];
    while (1) {
        uint8_t clr = (uint8_t) (myrand() & 0x0f);
        for (int i = 0; i < 3; i++) {
            v[i][0] = myrand() % width;
            v[i][1] = myrand() % height;
        }
        triangle(v[0], v[1], v[2], clr);
    }

    return 0;
}
