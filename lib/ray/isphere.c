/*
 * iSphere.c
 *
 *  Created on: Dec 9, 2018
 *      Author: jmiller
 */

#include "os.h"
#include "imath.h"
#include "isphere.h"

static fixed tmin = 1; // some small number

void sp_create(fixed x, fixed y, fixed z, fixed radius, iPhong* shader, iSphere* sp) {
    ivec3(x, y, z, &sp->center);
    sp->r2 = fmult(radius, radius);
    sp->shader = shader;
}

int sp_isect(iSphere* sp, iRay* ray, fixed* tmax) {
    // Distance from center to perpendicular plane
    Vec3i dir;
    isub3(&sp->center, &ray->point, &dir);
    fixed b = idot3(&ray->dir, &dir);
    fixed disc = fmult(b, b) - idot3(&dir, &dir) + sp->r2;

    int hit = 0;
    if (disc >= c_epsilon) {
        disc = fsqrt(disc);
        fixed t2 = b + disc;   // farthest point
        if (t2 > tmin && t2 < *tmax) {
            *tmax = t2;
            hit++;
        }
        fixed t1 = b - disc;   // nearest point
        if (t1 > tmin && t1 < *tmax) {
            *tmax = t1;
            hit++;
        }
    }
    return hit;
}

void sp_print(iSphere* sp) {
    printf("iSphere<%f, %f, %f>, radius2 = %f\n",
            toFloat(sp->center.x), toFloat(sp->center.y), toFloat(sp->center.z), toFloat(sp->r2));
}

void sp_normal(iSphere* sp, iRay* ray, fixed t, Vec3i* normal) {
    iaddscaled3(&ray->point, t, &ray->dir, normal);
    isub3(normal, &sp->center, normal);
    inormalize3(normal);
}

