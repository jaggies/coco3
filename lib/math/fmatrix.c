/*
 * fmatrix.c
 *
 *  Created on: Apr 28, 2019
 *      Author: jmiller
 */

#include "os.h"
#include "fmatrix.h"
#include "cc3gfx.h" // enum for X, Y, Z TODO

// Note that due to a compiler bug with cmoc, we can't use the Matrix typedef in declarations,
// so everything below takes a fixed[16] as a result :/

static const fixed identity[16] = {
    c_one, 0, 0, 0,
    0, c_one, 0, 0,
    0, 0, c_one, 0,
    0, 0, 0, c_one
};

enum fMatrixIndex {
    A11=0, A12=1, A13=2, A14=3,
    A21=4, A22=5, A23=6, A24=7,
    A31=8, A32=9, A33=10, A34=11,
    A41=12, A42=13, A43=14, A44=15 };

extern float Radians(float angle);

// Rotation matrix generation routines all use degrees
fixed* fRotationMatrix(float angle, uint8_t axis, fixed matrix[16]) {
    const float theta = Radians(angle);
    const fixed SIN = fromFloat(sin(theta));
    const fixed COS = fromFloat(cos(theta));
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

fixed* fRotationMatrixAxis(float angle, const fixed axis[3], fixed matrix[16])
{
    //const Vec3f a = normalize(axis);  // TODO
    const float theta = Radians(angle);
    const fixed s = fromFloat(sin(theta));
    const fixed c = fromFloat(cos(theta));
    const fixed x = axis[X], y = axis[Y], z = axis[Z];
    const fixed xx = fmult(x,x);
    const fixed xy = fmult(x,y);
    const fixed xz = fmult(x,z);
    const fixed yy = fmult(y,y);
    const fixed yz = fmult(y,z);
    const fixed zz = fmult(z,z);
    const fixed xs = fmult(x,s);
    const fixed ys = fmult(y,s);
    const fixed zs = fmult(z,s);
    const fixed omc = (c_one-c);
    const fixed m[16] = {
        fmult(xx,omc)+c, fmult(xy,omc)-zs, fmult(xz,omc)+ys, c_zero,
        fmult(xy,omc)+zs, fmult(yy,omc)+c, fmult(yz,omc)-xs, c_zero,
        fmult(xz,omc)-ys, fmult(yz,omc)+xs, fmult(zz,omc)+c, c_zero,
        c_zero,          c_zero,           c_zero,           c_zero };
    memcpy(matrix, m, sizeof(m));
    return matrix;
}

fixed* fTranslationMatrix(fixed tx, fixed ty, fixed tz, fixed matrix[16])
{
    memcpy(matrix, identity, sizeof(identity));
    matrix[A41] = tx;
    matrix[A42] = ty;
    matrix[A43] = tz;
    return matrix;
}

fixed* fScaleMatrix(fixed sx, fixed sy, fixed sz, fixed matrix[16])
{
    memcpy(matrix, identity, sizeof(identity));
    matrix[A11] = sx; //sx   0.0f 0.0f 0.0f
    matrix[A22] = sy; //0.0f sy   0.0f 0.0f
    matrix[A33] = sz; //0.0f 0.0f sz   0.0f
                      //0.0f 0.0f 0.0f 1.0f
    return matrix;
}

fixed* fTransposeMatrix(const fixed in[16], fixed out[16])
{
    fixed m[16] = {
        in[A11], in[A21], in[A31], in[A41],
        in[A12], in[A22], in[A32], in[A42],
        in[A13], in[A23], in[A33], in[A43],
        in[A14], in[A24], in[A34], in[A44] };
    memcpy(out, m, sizeof(m));
    return out;
}

fixed* fMultiplyMatrix(const fixed a[16], const fixed b[16], fixed out[16])
{
    fixed m[16] = {
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

fixed* fMultiplyScalar(const fixed a[16], const fixed b, fixed out[16]) {
    for (uint8_t i = 0; i < 16; i++) {
        *out++ = fmult(b, *a++);
    }
    return out;
}

fixed* fMatrixTransformVector(const fixed m[16], const fixed vin[3], fixed vout[3]) {
    fixed x = vin[X], y = vin[Y], z = vin[Z];
    vout[X] = fmult(x,m[A11]) + fmult(y,m[A21]) + fmult(z,m[A31]) + m[A41];
    vout[Y] = fmult(x,m[A12]) + fmult(y,m[A22]) + fmult(z,m[A32]) + m[A42];
    vout[Z] = fmult(x,m[A13]) + fmult(y,m[A23]) + fmult(z,m[A33]) + m[A43];
    return vout;
}

fixed* fMatrixTransformDirection(const fixed m[16], const fixed vin[3], fixed vout[3]) {
    fixed x = vin[X], y = vin[Y], z = vin[Z];
    vout[X] = fmult(x,m[A11]) + fmult(y,m[A21]) + fmult(z,m[A31]);
    vout[Y] = fmult(x,m[A12]) + fmult(y,m[A22]) + fmult(z,m[A32]);
    vout[Z] = fmult(x,m[A13]) + fmult(y,m[A23]) + fmult(z,m[A33]);
    return vout;
}

fixed* fMatrixLoadIdentity(fixed matrix[16]) {
    memcpy(matrix, identity, sizeof(identity));
    return matrix;
}

fixed* fMatrixPrint(fixed matrix[16]) {
    printf("%f %f %f %f\n", toFloat(matrix[0]), toFloat(matrix[1]), toFloat(matrix[2]), toFloat(matrix[3]));
    printf("%f %f %f %f\n", toFloat(matrix[4]), toFloat(matrix[5]), toFloat(matrix[6]), toFloat(matrix[7]));
    printf("%f %f %f %f\n", toFloat(matrix[8]), toFloat(matrix[9]), toFloat(matrix[10]), toFloat(matrix[11]));
    printf("%f %f %f %f\n", toFloat(matrix[12]), toFloat(matrix[13]), toFloat(matrix[14]), toFloat(matrix[15]));
    return matrix;
}



