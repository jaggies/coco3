/*
 * hit.c
 *
 *  Created on: Jan 26, 2019
 *      Author: jmiller
 */

#include "fixed.h"
#include "ivec3i.h"
#include "hit.h"

void init_hit(Hit* hit) {
    hit->t = c_max;
    hit->object = 0;
}

