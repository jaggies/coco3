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
    void* object; // hit object
} Hit;

#endif /* LIB_RAY_HIT_H_ */
