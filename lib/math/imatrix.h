/*
 * imatrix.h
 *
 *  Created on: Apr 28, 2019
 *      Author: jmiller
 */

#ifndef LIB_MATH_IMATRIX_H_
#define LIB_MATH_IMATRIX_H_

#include "fixed.h"

typedef fixed iMatrix[16];

// Creates a rotation matrix.  Returns pointer to matrix.
fixed* iRotationMatrix(float angle, uint8_t axis, fixed matrix[16]);

// Creates a translation matrix. Returns pointer to matrix.
fixed* iTranslationMatrix(fixed tx, fixed ty, fixed tz, fixed matrix[16]);

// Creates a scale matrix. Returns pointer to matrix.
fixed* iScaleMatrix(fixed sx, fixed sy, fixed sz, fixed matrix[16]);

// Creates a rotation matrix for the given axis. Returns pointer to matrix.
fixed* iRotationMatrixAxis(float angle, const fixed axis[3], fixed matrix[16]);

// Transposes the matrix. Returns pointer to matrix.
fixed* iTransposeMatrix(const fixed in[16], fixed out[16]);

// Multiplies matrix a by matrix b. Returns pointer to resulting output matrix.
fixed* iMultiplyMatrix(const fixed a[16], const fixed b[16], fixed out[16]);

// Multiplies matrix a by scalar b. Returns pointer to resulting output matrix.
fixed* iMultiplyScalar(const fixed a[16], const fixed b, fixed out[16]);

// Transforms the given vector by the matrix. Returns a pointer to the resulting matrix.
fixed* iMatrixTransformVector(const fixed matrix[16], const fixed vin[3], fixed vout[3]);

// Transforms the given vector by the matrix. Returns a pointer to the resulting matrix.
fixed* iMatrixTransformDirection(const fixed matrix[16], const fixed vin[3], fixed vout[3]);

// Creates an identity matrix. Returns pointer to matrix.
fixed* iMatrixLoadIdentity(fixed matrix[16]);

// Prints the given matrix.
fixed* iMatrixPrint(fixed matrix[16]);

#endif /* LIB_MATH_IMATRIX_H_ */
