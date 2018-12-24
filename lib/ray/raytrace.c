/*
 * raytrace.c
 *
 *  Created on: Dec 23, 2018
 *      Author: jmiller
 */
#include "fixed.h"
#include "ilight.h"
#include "isphere.h"
#include "raytrace.h"
#include "scene.h"

int raytrace(Scene* scene, iRay* ray, Hit* hit) {
    int hitCount = 0;
    // Find the nearest object
    hit->object = 0;
    for (int i = 0; i < scene->nSphere; i++) {
        iSphere* sphere = &scene->spheres[i];
        if (sp_isect(sphere, ray, &hit->t)) {
            hit->object = (void*) sphere;
            hitCount++;
        }
    }
    return hitCount;
}

void shade(Scene* scene, iRay* ray, Hit* hit, Vec3i* color) {
    iSphere *sp = (iSphere*) hit->object; // TODO: handle other object types
    Vec3i normal;
    sp_normal(sp, ray, hit->t, &normal);

    // Shade the hit object
    for (int i = 0; i < scene->nLight; i++) {
        iLight* light = &scene->lights[i];
        // Diffuse
        fixed cosAlpha = idot3(&light->direction, &normal);
        if (cosAlpha > 0) { // ignore back faces
           fixed scaledDiffuse = fmult(cosAlpha, sp->shader->kDiffuse);
           iaddscaled3(color, scaledDiffuse, &sp->shader->diffuse, color);
        }
    }
}
