/*
 * hit.h
 *
 *  Created on: Dec 23, 2018
 *      Author: jmiller
 */

#ifndef LIB_RAY_HIT_H_
#define LIB_RAY_HIT_H_

typedef struct _Hit {
    fixed t;
    Vec3i point; // Hit point
    Vec3i normal; // Normal at point
    Vec3i reflect; // Reflection direction
    void* object; // hit object
} Hit;

#endif /* LIB_RAY_HIT_H_ */
