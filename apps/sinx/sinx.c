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
#include "vec3.h"

#define WIDTH 320
#define HEIGHT 200
#define DEPTH 4
#define SIZE 128
#define LINES

#define NX 8
#define NY NX

static Vec3 points[NX][NY];
static Vec3 normals[NX][NY];
static Vec3 trans[NX][NY]; // transformed (tmp) vector in drawing

void simpleRGB() {
    for (uint8_t i = 0; i < 16; i++) {
        uint8_t red = (i & 1) ? 3 : 0;
        uint8_t grn = (i >> 1) & 3;
        uint8_t blu = ((i >> 3) & 1) ? 3 : 0;
        setPalette(i, red, grn, blu);
    }
}

static float sinx_x(float u, float v)
{
    float r = (u*u + v*v); // TODO: sqrt
    return 0.1f*sin(4.0f*r*M_PI) / r;
}

void computeSinX() {
    float u = -1.0f;
    float v = -1.0f;
    float du = 2.0f / (NX - 1);
    float dv = 2.0f / (NY - 1);

    for (int j = 0; j < NY; j++, v += dv, u = -1.0f) {
        for (int i = 0; i < NX; i++, u += du) {
            float ud = u + du * 0.1f;
            float vd = v + dv * 0.1f;
            Vec3 pdu, pdv, *point, *normal;
            point = &points[j][i];
            normal = &normals[j][i];
            vec3(u, sinx_x(u, v), v, point);
            vec3(ud, sinx_x(ud, v), v, &pdu);
            sub3(&pdu, point, &pdu);
            vec3(u, sinx_x(u, vd), vd, &pdv);
            sub3(&pdv, point, &pdv);
            cross(&pdv, &pdu, normal);
            normalize3(normal);
            printf("%f %f %f\n", point->x, point->y, point->z);
        }
    }
}

void drawSinX(float projection[16], int solid) {
    for (int j = 0; j < NY; j++) {
        for (int i = 0; i < NX; i++) {
            MatrixTransformVector(projection,
                    (const float*) &points[j][i].x, (float*) &trans[j][i].x);
        }
    }
    for (int j = 0; j < NY - 1; j++) {
        for (int i = 0; i < NX - 1; i++) {
            int p0[2], p1[2], p2[2], p3[2];
            p0[0] = (int) trans[j][i].x;
            p1[0] = (int) trans[j][i + 1].x;
            p2[0] = (int) trans[j + 1][i].x;
            p3[0] = (int) trans[j + 1][i + 1].x;

            p0[1] = (int) trans[j][i].y;
            p1[1] = (int) trans[j][i + 1].y;
            p2[1] = (int) trans[j + 1][i].y;
            p3[1] = (int) trans[j + 1][i + 1].y;

            rasterColor(i^j);
            triangle(p1, p0, p2);
            triangle(p2, p3, p1);
        }
    }
}

int main(int argc, char** argv) {
    /* Speedups */
    set6309Native();
    initCoCoSupport();
    setHighSpeed(1);

    /* Compute first! */
    printf("Computing sinx()/x\n");
    computeSinX();

    /* Graphics */
    setMode(WIDTH, HEIGHT, DEPTH);
    simpleRGB();

    /* Draw pixels */
    const uint8_t clearColor = 0x08; // blue
    clear(clearColor);

    int alpha = 0;
    int beta = 8;
    int gamma = 0;
    int frame = 0;

    const int32_t gfxBase[2] = { 0x00000, 0x10000 };

    float model[16], projection[16], r1[16], r2[16], r3[16], tmp1[16];

    MultiplyMatrix(
            ScaleMatrix(SIZE, -SIZE, SIZE, projection),
            TranslationMatrix(WIDTH / 2, HEIGHT / 2, 0, tmp1),
            projection);

    MultiplyMatrix(
            RotationMatrix((float) -22.0f, X, r1),
            projection,
            projection);

    MultiplyMatrix(
            RotationMatrix((float) alpha, X, r1),
            RotationMatrix((float) beta, Y, r2),
            tmp1);

    MultiplyMatrix(
            RotationMatrix((float) gamma, Z, r3),
            tmp1,
            model);


    printf("Rendering...\n");
    int i = 0;
    while (1) {
        setGraphicsDrawBase(gfxBase[frame & 1]);
        clear(8);
        drawSinX(projection, 1);
        for (uint8_t j = 0; j < NY; j++) {
            for (uint8_t i = 0; i < NX; i++) {
                MatrixTransformVector(model,
                        (const float*) &points[j][i].x, (float*) &points[j][i].x);
            }
        }
        int t = getTimer();
        while (getTimer() == t) // wait for vsync
            ;
        setGraphicsViewBase(gfxBase[frame & 1]);
        frame++;
    }

    return 0;
}
