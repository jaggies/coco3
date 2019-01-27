/*
 * hit.h
 *
 *  Created on: Dec 23, 2018
 *      Author: jmiller
 */

#ifndef LIB_RAY_HIT_H_
#define LIB_RAY_HIT_H_

enum ObjectType{ TYPE_UKNOWN, TYPE_SPHERE, TYPE_TRIANGLE };

typedef struct _iHit {
    fixed t;
    Vec3i point; // Hit point
    Vec3i normal; // Normal at point
    Vec3i reflect; // Reflection direction
    void* object; // hit object
    int   type; // object type
} iHit;

extern void init_hit(iHit* hit);

#endif /* LIB_RAY_HIT_H_ */
