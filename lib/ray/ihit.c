/*
 * hit.c
 *
 *  Created on: Jan 26, 2019
 *      Author: jmiller
 */

#include "os.h"
#include "fixed.h"
#include "ivec3i.h"
#include "ihit.h"

void init_hit(iHit* hit) {
    hit->t = c_max;
    hit->object = 0;
    hit->type = TYPE_UKNOWN;
}

