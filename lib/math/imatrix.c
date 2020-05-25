/*
 * imatrix.c
 *
 *  Created on: Apr 28, 2019
 *      Author: jmiller
 */

#include "os.h"
#include "imatrix.h"
#include "cc3gfx.h" // enum for X, Y, Z TODO

// Note that due to a compiler bug with cmoc, we can't use the Matrix typedef in declarations,
// so everything below takes a int16_t[16] as a result :/

static const int16_t identity[16] = {
    c_one, 0, 0, 0,
    0, c_one, 0, 0,
    0, 0, c_one, 0,
    0, 0, 0, c_one
};

enum iMatrixIndex {
    A11=0, A12=1, A13=2, A14=3,
    A21=4, A22=5, A23=6, A24=7,
    A31=8, A32=9, A33=10, A34=11,
    A41=12, A42=13, A43=14, A44=15 };

extern float Radians(float angle);

// Rotation matrix generation routines all use degrees
int16_t* iRotationMatrix(float angle, uint8_t axis, int16_t matrix[16]) {
    const float theta = Radians(angle);
    const int16_t SIN = fromFloat(sin(theta));
    const int16_t COS = fromFloat(cos(theta));
    memcpy(matrix, identity, sizeof(identity));
    if (axis == X) {
        matrix[A22] = COS;  //1.0f 0.0f 0.0f 0.0f
        matrix[A23] = -SIN; //0.0f COS  SIN  0.0f
        matrix[A32] = SIN;  //0.0f -SIN COS  0.0f
        matrix[A33] = COS;  //0.0f 0.0f 0.0f 1.0f
    } else if (axis == Y) {
        matrix[A11] = COS;  //COS  0.0f -SIN 0.0f
        matrix[A13] = SIN;  //0.0f 1.0f 0.0f 0.0f
        matrix[A31] = -SIN; //SIN  0.0f COS  0.0f
        matrix[A33] = COS;  //0.0f 0.0f 0.0f 1.0f
    } else if (axis == Z) {
        matrix[A11] = COS;  // COS SIN  0.0f 0.0f
        matrix[A12] = -SIN; //-SIN COS  0.0f 0.0f
        matrix[A21] = SIN;  //0.0f 0.0f 1.0f 0.0f
        matrix[A22] = COS;  //0.0f 0.0f 0.0f 1.0f
    }
    return matrix;
}

int16_t* iRotationMatrixAxis(float angle, const int16_t axis[3], int16_t matrix[16])
{
    //const Vec3f a = normalize(axis);  // TODO
    const float theta = Radians(angle);
    const int16_t s = fromFloat(sin(theta));
    const int16_t c = fromFloat(cos(theta));
    const int16_t x = axis[X], y = axis[Y], z = axis[Z];
    const int16_t xx = fmult(x,x);
    const int16_t xy = fmult(x,y);
    const int16_t xz = fmult(x,z);
    const int16_t yy = fmult(y,y);
    const int16_t yz = fmult(y,z);
    const int16_t zz = fmult(z,z);
    const int16_t xs = fmult(x,s);
    const int16_t ys = fmult(y,s);
    const int16_t zs = fmult(z,s);
    const int16_t omc = (c_one-c);
    const int16_t m[16] = {
        fmult(xx,omc)+c, fmult(xy,omc)-zs, fmult(xz,omc)+ys, c_zero,
        fmult(xy,omc)+zs, fmult(yy,omc)+c, fmult(yz,omc)-xs, c_zero,
        fmult(xz,omc)-ys, fmult(yz,omc)+xs, fmult(zz,omc)+c, c_zero,
        c_zero,          c_zero,           c_zero,           c_zero };
    memcpy(matrix, m, sizeof(m));
    return matrix;
}

int16_t* iTranslationMatrix(int16_t tx, int16_t ty, int16_t tz, int16_t matrix[16])
{
    memcpy(matrix, identity, sizeof(identity));
    matrix[A41] = tx;
    matrix[A42] = ty;
    matrix[A43] = tz;
    return matrix;
}

int16_t* iScaleMatrix(int16_t sx, int16_t sy, int16_t sz, int16_t matrix[16])
{
    memcpy(matrix, identity, sizeof(identity));
    matrix[A11] = sx; //sx   0.0f 0.0f 0.0f
    matrix[A22] = sy; //0.0f sy   0.0f 0.0f
    matrix[A33] = sz; //0.0f 0.0f sz   0.0f
                      //0.0f 0.0f 0.0f 1.0f
    return matrix;
}

int16_t* iTransposeMatrix(const int16_t in[16], int16_t out[16])
{
    int16_t m[16] = {
        in[A11], in[A21], in[A31], in[A41],
        in[A12], in[A22], in[A32], in[A42],
        in[A13], in[A23], in[A33], in[A43],
        in[A14], in[A24], in[A34], in[A44] };
    memcpy(out, m, sizeof(m));
    return out;
}

int16_t* iMultiplyMatrix(const int16_t a[16], const int16_t b[16], int16_t out[16])
{
    int16_t m[16] = {
        fmult(a[A11],b[A11])+fmult(a[A12],b[A21])+fmult(a[A13],b[A31])+fmult(a[A14],b[A41]),
        fmult(a[A11],b[A12])+fmult(a[A12],b[A22])+fmult(a[A13],b[A32])+fmult(a[A14],b[A42]),
        fmult(a[A11],b[A13])+fmult(a[A12],b[A23])+fmult(a[A13],b[A33])+fmult(a[A14],b[A43]),
        fmult(a[A11],b[A14])+fmult(a[A12],b[A24])+fmult(a[A13],b[A34])+fmult(a[A14],b[A44]),

        fmult(a[A21],b[A11])+fmult(a[A22],b[A21])+fmult(a[A23],b[A31])+fmult(a[A24],b[A41]),
        fmult(a[A21],b[A12])+fmult(a[A22],b[A22])+fmult(a[A23],b[A32])+fmult(a[A24],b[A42]),
        fmult(a[A21],b[A13])+fmult(a[A22],b[A23])+fmult(a[A23],b[A33])+fmult(a[A24],b[A43]),
        fmult(a[A21],b[A14])+fmult(a[A22],b[A24])+fmult(a[A23],b[A34])+fmult(a[A24],b[A44]),

        fmult(a[A31],b[A11])+fmult(a[A32],b[A21])+fmult(a[A33],b[A31])+fmult(a[A34],b[A41]),
        fmult(a[A31],b[A12])+fmult(a[A32],b[A22])+fmult(a[A33],b[A32])+fmult(a[A34],b[A42]),
        fmult(a[A31],b[A13])+fmult(a[A32],b[A23])+fmult(a[A33],b[A33])+fmult(a[A34],b[A43]),
        fmult(a[A31],b[A14])+fmult(a[A32],b[A24])+fmult(a[A33],b[A34])+fmult(a[A34],b[A44]),

        fmult(a[A41],b[A11])+fmult(a[A42],b[A21])+fmult(a[A43],b[A31])+fmult(a[A44],b[A41]),
        fmult(a[A41],b[A12])+fmult(a[A42],b[A22])+fmult(a[A43],b[A32])+fmult(a[A44],b[A42]),
        fmult(a[A41],b[A13])+fmult(a[A42],b[A23])+fmult(a[A43],b[A33])+fmult(a[A44],b[A43]),
        fmult(a[A41],b[A14])+fmult(a[A42],b[A24])+fmult(a[A43],b[A34])+fmult(a[A44],b[A44]) };
    memcpy(out, m, sizeof(m));
    return out;
}

int16_t* iMultiplyScalar(const int16_t a[16], const int16_t b, int16_t out[16]) {
    for (uint8_t i = 0; i < 16; i++) {
        *out++ = fmult(b, *a++);
    }
    return out;
}

int16_t* iMatrixTransformVector(const int16_t m[16], const int16_t vin[3], int16_t vout[3]) {
    int16_t x = vin[X], y = vin[Y], z = vin[Z];
    vout[X] = fmult(x,m[A11]) + fmult(y,m[A21]) + fmult(z,m[A31]) + m[A41];
    vout[Y] = fmult(x,m[A12]) + fmult(y,m[A22]) + fmult(z,m[A32]) + m[A42];
    vout[Z] = fmult(x,m[A13]) + fmult(y,m[A23]) + fmult(z,m[A33]) + m[A43];
    return vout;
}

int16_t* iMatrixTransformDirection(const int16_t m[16], const int16_t vin[3], int16_t vout[3]) {
    int16_t x = vin[X], y = vin[Y], z = vin[Z];
    vout[X] = fmult(x,m[A11]) + fmult(y,m[A21]) + fmult(z,m[A31]);
    vout[Y] = fmult(x,m[A12]) + fmult(y,m[A22]) + fmult(z,m[A32]);
    vout[Z] = fmult(x,m[A13]) + fmult(y,m[A23]) + fmult(z,m[A33]);
    return vout;
}

int16_t* iMatrixLoadIdentity(int16_t matrix[16]) {
    memcpy(matrix, identity, sizeof(identity));
    return matrix;
}

int16_t* iMatrixPrint(int16_t matrix[16]) {
    printf("%f %f %f %f\n", toFloat(matrix[0]), toFloat(matrix[1]), toFloat(matrix[2]), toFloat(matrix[3]));
    printf("%f %f %f %f\n", toFloat(matrix[4]), toFloat(matrix[5]), toFloat(matrix[6]), toFloat(matrix[7]));
    printf("%f %f %f %f\n", toFloat(matrix[8]), toFloat(matrix[9]), toFloat(matrix[10]), toFloat(matrix[11]));
    printf("%f %f %f %f\n", toFloat(matrix[12]), toFloat(matrix[13]), toFloat(matrix[14]), toFloat(matrix[15]));
    return matrix;
}



