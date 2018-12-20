/*
 * iray.h
 *
 *  Created on: Dec 9, 2018
 *      Author: jmiller
 */

#ifndef APPS_INTRAY_IRAY_H_
#define APPS_INTRAY_IRAY_H_

#include "ivec3i.h"

typedef struct _iRay {
    Vec3i point;
    Vec3i dir;
} iRay;

#endif /* APPS_INTRAY_IRAY_H_ */
