/*
 * palette.c
 *
 *  Created on: Feb 12, 2019
 *      Author: jmiller
 */

#include "os.h"
#include "cc3hw.h"
#include "cc3gfx.h"
#include "cc3line.h"
#include "cc3triangle.h"
#include "cc3raster.h"
#include "matrix.h"

#define WIDTH 320
#define HEIGHT 200
#define DEPTH 4
#define CUBESIZE 128
#define LINES

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

static float vertex[8][3] = {
    {cubeMin[X], cubeMin[Y], cubeMin[Z]}, {cubeMax[X], cubeMin[Y], cubeMin[Z]},
    {cubeMax[X], cubeMin[Y], cubeMax[Z]}, {cubeMin[X], cubeMin[Y], cubeMax[Z]},
    {cubeMin[X], cubeMax[Y], cubeMin[Z]}, {cubeMax[X], cubeMax[Y], cubeMin[Z]},
    {cubeMax[X], cubeMax[Y], cubeMax[Z]}, {cubeMin[X], cubeMax[Y], cubeMax[Z]} };

// bottom, top, front, back, left, right - in CW order
static const uint8_t faces[6][4] = {
        {3, 2, 1, 0}, {4, 5, 6, 7}, {2, 3, 7, 6},
        {0, 4, 5, 1}, {0, 4, 7, 3}, {1, 5, 6, 2} };

static const uint8_t vertex2face[8][3] = {
        {0, 3, 4}, {0, 3, 5}, {0, 2, 5}, {0, 2, 4},
        {1, 3, 4}, {1, 3, 5}, {1, 2, 5}, {1, 2, 4}
};

static const uint8_t wire[12][2] = {
        {0,1}, {1,2}, {2, 3}, {3, 0},
        {0,4}, {1,5}, {2, 6}, {3, 7},
        {4,5}, {5,6}, {6, 7}, {7, 4}};

//static const float normals[6][3] = {
//        {0.0f, 1.0f, 0.0f},  {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f, -1.0f},
//        {0.0f, 0.0f, -1.0f}, {1.0f,  0.0f, 0.0f}, {1.0f, 0.0f,  0.0f} };

void drawFace(float v0[3], float v1[3], float v2[3], float v3[3]) {
    int p0[2], p1[2], p2[2], p3[2];
    for (int i = 0; i < 2; i++) {
        p0[i] = (int) v0[i];
        p1[i] = (int) v1[i];
        p2[i] = (int) v2[i];
        p3[i] = (int) v3[i];
    }
#ifdef LINES
    line(p0[X], p0[Y], p1[X], p1[Y]);
    line(p1[X], p1[Y], p2[X], p2[Y]);
    line(p2[X], p2[Y], p3[X], p3[Y]);
    line(p3[X], p3[Y], p0[X], p0[Y]);
#else
    triangle(p0, p1, p2);
    triangle(p0, p2, p3);
#endif
}

float dot(const float a[3], const float b[3]) {
    return a[X] * b[X] + a[Y] * b[Y] + a[Z] * b[Z];
}

void drawCube(float projection[16], int solid)
{
    float vtrans[8][3];
    for (uint8_t i = 0; i < 8; i++) {
        MatrixTransformVector(projection, vertex[i], vtrans[i]);
    }

    if (solid) {
        // Find the point closest to the camera and draw the faces attached to it
        uint8_t bestVertex = 0;
        for (uint8_t i = 1; i < 8; i++) {
            if (vtrans[i][Z] < vtrans[bestVertex][Z]) {
                bestVertex = i;
            }
        }
        const uint8_t * flist = vertex2face[bestVertex];
        for (uint8_t f = 0; f < 3; f++) {
            uint8_t face = *flist++;
            rasterColor(face + 1);
            uint8_t* f = faces[face];
            drawFace(vtrans[f[0]], vtrans[f[1]], vtrans[f[2]], vtrans[f[3]]);
        }
    } else {
        for (uint8_t i = 0; i < 12; i++) {
            rasterColor(0x0f);
            float* p1 = &vtrans[wire[i][0]];
            float* p2 = &vtrans[wire[i][1]];
            line((int)p1[X], (int)p1[Y], (int)p2[X], (int)p2[Y]);
        }
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
    clear(clearColor);

    int alpha = 6;
    int beta = 8;
    int gamma = 10;
    int frame = 0;

    const int32_t gfxBase[2] = { 0x00000, 0x10000 };

    float model[16], projection[16], r1[16], r2[16], r3[16], tmp1[16];

    MultiplyMatrix(
            ScaleMatrix(CUBESIZE, CUBESIZE, 1, projection),
            TranslationMatrix(WIDTH / 2, HEIGHT / 2, 0, tmp1),
            projection);

    MultiplyMatrix(
            RotationMatrix((float) alpha, X, r1),
            RotationMatrix((float) beta, Y, r2),
            tmp1);
    MultiplyMatrix(
            RotationMatrix((float) gamma, Z, r3),
            tmp1,
            model);

    while (1) {
        setGraphicsDrawBase(gfxBase[frame & 1]);
        clear(clearColor);
        drawCube(projection, 1);
        for (uint8_t i = 0; i < 8; i++) {
            MatrixTransformVector(model, vertex[i], vertex[i]);
        }
        int t = getTimer();
        while (getTimer() == t) // wait for vsync
            ;
        setGraphicsViewBase(gfxBase[frame & 1]);
        frame++;
    }

    return 0;
}
