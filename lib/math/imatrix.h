/*
 * imatrix.h
 *
 *  Created on: Apr 28, 2019
 *      Author: jmiller
 */

#ifndef LIB_MATH_IMATRIX_H_
#define LIB_MATH_IMATRIX_H_

typedef int16_t iMatrix[16];

// Creates a rotation matrix.  Returns pointer to matrix.
int16_t* iRotationMatrix(float angle, uint8_t axis, int16_t matrix[16]);

// Creates a translation matrix. Returns pointer to matrix.
int16_t* iTranslationMatrix(int16_t tx, int16_t ty, int16_t tz, int16_t matrix[16]);

// Creates a scale matrix. Returns pointer to matrix.
int16_t* iScaleMatrix(int16_t sx, int16_t sy, int16_t sz, int16_t matrix[16]);

// Creates a rotation matrix for the given axis. Returns pointer to matrix.
int16_t* iRotationMatrixAxis(float angle, const int16_t axis[3], int16_t matrix[16]);

// Transposes the matrix. Returns pointer to matrix.
int16_t* iTransposeMatrix(const int16_t in[16], int16_t out[16]);

// Multiplies matrix a by matrix b. Returns pointer to resulting output matrix.
int16_t* iMultiplyMatrix(const int16_t a[16], const int16_t b[16], int16_t out[16]);

// Multiplies matrix a by scalar b. Returns pointer to resulting output matrix.
int16_t* iMultiplyScalar(const int16_t a[16], const int16_t b, int16_t out[16]);

// Transforms the given vector by the matrix. Returns a pointer to the resulting matrix.
int16_t* iMatrixTransformVector(const int16_t matrix[16], const int16_t vin[3], int16_t vout[3]);

// Transforms the given vector by the matrix. Returns a pointer to the resulting matrix.
int16_t* iMatrixTransformDirection(const int16_t matrix[16], const int16_t vin[3], int16_t vout[3]);

// Creates an identity matrix. Returns pointer to matrix.
int16_t* iMatrixLoadIdentity(int16_t matrix[16]);

// Prints the given matrix.
int16_t* iMatrixPrint(int16_t matrix[16]);

#endif /* LIB_MATH_IMATRIX_H_ */
