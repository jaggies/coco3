/*
 * light.c
 *
 *  Created on: Dec 23, 2018
 *      Author: jmiller
 */

#include "os.h"
#include "ilight.h"

void light_create(Vec3i* dir, Vec3i* color, iLight* light) {
    icopy3(dir, &light->direction);
    inormalize3(&light->direction);
    icopy3(color, &light->color);
    //printf("Light dir: "); ivec3_print(&light->direction);
}
