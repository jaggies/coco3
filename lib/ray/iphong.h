/*
 * phong.h
 *
 *  Created on: Dec 23, 2018
 *      Author: jmiller
 */

#ifndef LIB_RAY_PHONG_H_
#define LIB_RAY_PHONG_H_

typedef struct _Phong {
    Vec3i diffuse;
    Vec3i specular;
    fixed kDiffuse;
    fixed kSpecular;
    fixed kReflect;
    fixed coefficient; // Specular power
    fixed index; // Index of refraction
} iPhong;

void make_phong(Vec3i* diff, Vec3i* spec, fixed kDiff, fixed kSpecular, fixed kReflect, fixed coeff,
        fixed index, iPhong* phong);

#endif /* LIB_RAY_PHONG_H_ */
