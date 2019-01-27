/*
 * itriangle.c
 *
 *  Created on: Jan 27, 2019
 *      Author: jmiller
 */

#include "itriangle.h"

static fixed alphaSave;
static fixed betaSave;

void tri_create(Vec3i* p0, Vec3i* p1, Vec3i* p2, iPhong* shader, iTriangle* triangle) {
    triangle->shader = shader;
    icopy3(p0, &triangle->point);
    isub3(p1, p0, &triangle->edge[0]);
    isub3(p2, p0, &triangle->edge[1]);
    icross(&triangle->edge[0], &triangle->edge[1], &triangle->normal);
    inormalize3(&triangle->normal);
}

int tri_isect(iTriangle* triangle, iRay* ray, fixed* tmax) {
    Vec3i s1; icross(&ray->dir, &triangle->edge[1], &s1);
    fixed div = idot3(&s1, &triangle->edge[0]);
    if (div == c_zero) return 0;  // ray parallel to plane.

    fixed invDiv = fdiv(c_one, div);
    Vec3i d; isub3(&ray->point, &triangle->point, &d);

    // Compute barycentric coordinate 1
    fixed alpha = fmult(idot3(&d, &s1), invDiv);
    if ((alpha < c_zero) || (alpha > c_one)) return 0;

    // Compute barycentric coordinate 2
    Vec3i s2; icross(&d, &triangle->edge[0], &s2);
    fixed beta = fmult(idot3(&ray->dir, &s2), invDiv);
    if ((beta < c_zero) || ((alpha + beta) > c_one)) return 0;

    fixed t = fmult(idot3(&triangle->edge[1], &s2), invDiv);

    if ((t > c_zero) && (t < *tmax)) {
       *tmax = t;
       alphaSave = alpha;
       betaSave = beta;
       return 1;
    }
    return 0;
}

void tri_normal(iTriangle* triangle, iRay* ray, fixed t, Vec3i* outPoint, Vec3i* outNormal) {
    iaddscaled3(&ray->point, t, &ray->dir, outPoint);
    icopy3(&triangle->normal, outNormal);
}

void tri_print(iTriangle* triangle) {

}
