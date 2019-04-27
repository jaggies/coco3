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
#include "matrix.h"

#define WIDTH 320
#define HEIGHT 225
#define DEPTH 4

void simpleRGB() {
    for (uint8_t i = 0; i < 16; i++) {
        uint8_t red = (i & 1) ? 3 : 0;
        uint8_t grn = (i >> 1) & 3;
        uint8_t blu = ((i >> 3) & 1) ? 3 : 0;
        setPalette(i, red, grn, blu);
    }
}

void printvec(float v[3]) {
    printf("<%f %f %f>\n", v[X], v[Y], v[Z]);
}

void testMatrix() {

    const float v[3] = { 1, 2, 3 };
    float vout[3];

    float m[16];
    MatrixLoadIdentity(m);
    ScaleMatrix(0.1f, 0.1f, 0.1f, m);
    MatrixPrint(m);

    MatrixTransformVector(m, v, vout);
    printvec(vout);

    float t[16];
    MatrixTransformVector(TranslationMatrix(4, 5, 6, t), v, vout);
    printvec(vout);

    float r[16];
    MatrixTransformVector(RotationMatrix(90, Y, r), v, vout);
    printvec(vout);
}

static const float cubeMin[3] = { -0.5f, -0.5f, -0.5f };
static const float cubeMax[3] = {  0.5f, 0.5f, 0.5f };

static const float vertex[8][3] = {
    {cubeMin[X], cubeMin[Y], cubeMin[Z]}, {cubeMax[X], cubeMin[Y], cubeMin[Z]},
    {cubeMax[X], cubeMin[Y], cubeMax[Z]}, {cubeMin[X], cubeMin[Y], cubeMax[Z]},
    {cubeMin[X], cubeMax[Y], cubeMin[Z]}, {cubeMax[X], cubeMax[Y], cubeMin[Z]},
    {cubeMax[X], cubeMax[Y], cubeMax[Z]}, {cubeMin[X], cubeMax[Y], cubeMax[Z]} };

// bottom, top, front, back, left, right - in CW order
static const int faces[6][4]= {
        {3, 2, 1, 0}, {4, 5, 6, 7}, {2, 3, 7, 6},
        {0, 4, 5, 1}, {0, 4, 7, 3}, {1, 5, 6, 2} };

//static const float normals[6][3] = {
//        {0.0, 1.0, 0.0}, {0.0, -1.0, 0.0}, {0.0, 0.0, -1.0},
//        {0.0, 0.0, -1.0}, {1.0, 0.0, 0.0}, {1.0, 0.0, 0.0} };


void drawFace(float v0[3], float v1[3], float v2[3], float v3[3]) {
    int p0[2], p1[2], p2[2], p3[2];
    int s[2] = { 50, 50 };
    int t[2] = { WIDTH / 2, HEIGHT / 2 };
    for (int i = 0; i < 2; i++) {
        p0[i] = s[i] * v0[i] + t[i];
        p1[i] = s[i] * v1[i] + t[i];
        p2[i] = s[i] * v2[i] + t[i];
        p3[i] = s[i] * v3[i] + t[i];
    }
    triangle(p0, p1, p2);
    triangle(p0, p2, p3);
}

void drawCube(float m[16])
{
    // Transform vertices
    float v[8][3];
    for (uint8_t i = 0; i < 8; i++) {
        MatrixTransformVector(m, vertex[i], v[i]);
    }
    for (uint8_t i = 0; i < 6; i++) {
        rasterColor(i + 1);
        drawFace(v[faces[i][0]], v[faces[i][1]], v[faces[i][2]], v[faces[i][3]]);
    }
}

int main(int argc, char** argv) {
    /* Speedups */
    set6309Native();
    initCoCoSupport();
    setHighSpeed(1);

    testMatrix();

    /* Graphics */
    setMode(WIDTH, HEIGHT, DEPTH);
    simpleRGB();

    /* Draw pixels */
    const uint8_t clearColor = 0x08; // blue

    float m[16];
    MatrixLoadIdentity(m);
    int angle = 0;
    while (1) {
        clear(clearColor);
        RotationMatrix((float) angle, Y, m);
        drawCube(m);
        angle = (angle + 1) % 360;
    }

    return 0;
}
