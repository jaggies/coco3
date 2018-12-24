/*
 * iphong.c
 *
 *  Created on: Dec 23, 2018
 *      Author: jmiller
 */

void make_phong(Vec3i* diff, Vec3i* spec, fixed kDiff, fixed kSpecular, fixed coeff, fixed index,
        iPhong* phong) {
    icopy3(diff, phong->diffuse);
    icopy3(spec, phong->specular);
    phong->kDiffuse = kDiff;
    phong->kSpecular = kSpecular;
    phong->coefficient = coeff;
}


