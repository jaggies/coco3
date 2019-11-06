/*
 * imatrix.h
 *
 *  Created on: Apr 28, 2019
 *      Author: jmiller
 */

#ifndef LIB_MATH_FMATRIX_H_
#define LIB_MATH_FMATRIX_H_

#include "fixed.h"

typedef fixed fMatrix[16];

// Creates a rotation matrix.  Returns pointer to matrix.
fixed* fRotationMatrix(float angle, uint8_t axis, fixed matrix[16]);

// Creates a translation matrix. Returns pointer to matrix.
fixed* fTranslationMatrix(fixed tx, fixed ty, fixed tz, fixed matrix[16]);

// Creates a scale matrix. Returns pointer to matrix.
fixed* fScaleMatrix(fixed sx, fixed sy, fixed sz, fixed matrix[16]);

// Creates a rotation matrix for the given axis. Returns pointer to matrix.
fixed* fRotationMatrixAxis(float angle, const fixed axis[3], fixed matrix[16]);

// Transposes the matrix. Returns pointer to matrix.
fixed* fTransposeMatrix(const fixed in[16], fixed out[16]);

// Multiplies matrix a by matrix b. Returns pointer to resulting output matrix.
fixed* fMultiplyMatrix(const fixed a[16], const fixed b[16], fixed out[16]);

// Multiplies matrix a by scalar b. Returns pointer to resulting output matrix.
fixed* fMultiplyScalar(const fixed a[16], const fixed b, fixed out[16]);

// Transforms the given vector by the matrix. Returns a pointer to the resulting matrix.
fixed* fMatrixTransformVector(const fixed matrix[16], const fixed vin[3], fixed vout[3]);

// Transforms the given vector by the matrix. Returns a pointer to the resulting matrix.
fixed* fMatrixTransformDirection(const fixed matrix[16], const fixed vin[3], fixed vout[3]);

// Creates an identity matrix. Returns pointer to matrix.
fixed* fMatrixLoadIdentity(fixed matrix[16]);

// Prints the given matrix.
fixed* fMatrixPrint(fixed matrix[16]);

#endif /* LIB_MATH_FMATRIX_H_ */
