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
#include "cc3raster.h"

#define WIDTH 320
#define HEIGHT 225
#define VBLANK (262 - HEIGHT)
#define DEPTH 4
#define TRI_SIZE 200 // pixels

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
    rasterColor(clr);
    triangle(&v[0][0], &v[1][0], &v[2][0]);
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
}

void testCases() {
    Vertex flatt[] = { { TRI_SIZE/2, TRI_SIZE}, { 0, 0 }, { TRI_SIZE, 0 } };
    Vertex flatb[] = { { TRI_SIZE/2, 0}, { 0, TRI_SIZE }, { TRI_SIZE, TRI_SIZE } };
    Vertex left[] = { { TRI_SIZE, 0}, { 0, TRI_SIZE/2 }, { TRI_SIZE, TRI_SIZE } };
    Vertex right[] = { { 0, 0}, { TRI_SIZE, TRI_SIZE/2 }, { 0, TRI_SIZE } };
    int8_t cases[][3] = { {0, 1, 2}, {0, 2, 1}, {1, 0, 2}, {1, 2, 0}, {2, 0, 1}, {2, 1, 0} };
    for (uint8_t i = 0; i < 6; i++) {
        rasterColor(i);
        triangle(&flatb[cases[i][0]].x, &flatb[cases[i][1]].x, &flatb[cases[i][2]].x);
    }
    for (uint8_t i = 0; i < 6; i++) {
        rasterColor(i);
        triangle(&flatt[cases[i][0]].x, &flatt[cases[i][1]].x, &flatt[cases[i][2]].x);
    }
    for (uint8_t i = 0; i < 6; i++) {
        rasterColor(i);
        triangle(&left[cases[i][0]].x, &left[cases[i][1]].x, &left[cases[i][2]].x);
    }
    for (uint8_t i = 0; i < 6; i++) {
        rasterColor(i);
        triangle(&right[cases[i][0]].x, &right[cases[i][1]].x, &right[cases[i][2]].x);
    }
}

void randomTriangles(int count) {
    const int height = getHeight();
    const int width = getWidth();
    while (--count) {
        int16_t vert[3][2];
        for (int i = 0; i < 3; i++) {
            vert[i][0] = myrand() % width;
            vert[i][1] = myrand() % height;
        }
        rasterColor((uint8_t) (myrand() & 0x0f));
        triangle(vert[0], vert[1], vert[2]);
    }
}

void rotatingTriangles() {
    const int r = 100;
    const float pi2 = 2.0f * M_PI;
    const float pi13 = pi2 / 3;
    const float dAlpha = pi2 / 360;
    float alpha = 0;
    for (int8_t i = 0; i < 100; i++) {
        int vert[3][2];
        for (int8_t v = 0; v < 3; v++) {
            float a = pi13 * v;
            vert[v][0] = (int) (i * sin(a + alpha) + r);
            vert[v][1] = (int) (i * cos(a + alpha) + r);
        }
        rasterColor((uint8_t)i);
        triangle(vert[0], vert[1], vert[2]);
        alpha += dAlpha;
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

    /* Draw pixels */
    const uint8_t clearColor = 0x08; // blue
    while (1) {
        clear(clearColor);
        testCases();
        clear(clearColor);
        testpoly();
        clear(clearColor);
        rotatingTriangles();
        clear(clearColor);
        randomTriangles(200);
    }

    return 0;
}
