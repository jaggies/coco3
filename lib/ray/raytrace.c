/*
 * trace.c
 *
 *  Created on: Dec 23, 2018
 *      Author: jmiller
 */
#include "os.h"
#include "fixed.h"
#include "ilight.h"
#include "isphere.h"
#include "raytrace.h"
#include "scene.h"

void updateHit(iSphere* object, iRay* ray, Hit* hit) {
    iaddscaled3(&ray->point, hit->t, &ray->dir, &hit->point);
    sp_normal(object, ray, hit->t, &hit->point, &hit->normal);
    ireflect3(&ray->dir, &hit->normal, &hit->reflect);
}

int trace(Scene* scene, iRay* ray, Hit* hit) {
    int hitCount = 0;
    // Find the nearest object
    hit->object = 0;
    for (int i = 0; i < scene->nSphere; i++) {
        iSphere* sphere = &scene->spheres[i];
        if (hit->object != (void*) sphere && sp_isect(sphere, ray, &hit->t)) {
            hit->object = (void*) sphere;
            hitCount++;
        }
    }
    return hitCount;
}

void shade(Scene* scene, iRay* ray, Hit* hit, Vec3i* color, uint8_t depth) {
    iSphere *sp = (iSphere*) hit->object; // TODO: handle other object types
    updateHit(sp, ray, hit);

    Vec3i vertexToEye;
    if (sp->shader->kSpecular > c_zero) {
        isub3(&hit->point, &ray->point, &vertexToEye);
        inormalize3(&vertexToEye);
    }

    // Shade the hit object
    for (int i = 0; i < scene->nLight; i++) {
        Vec3i reflect;
        iLight* light = &scene->lights[i];
        // Diffuse
        if (sp->shader->kDiffuse > c_zero) {
            fixed cosAlpha = idot3(&light->direction, &hit->normal);
            if (cosAlpha > 0) { // ignore back faces
                Vec3i diffuseColor;
                imult3(&light->color, &sp->shader->diffuse, &diffuseColor);
                fixed scaledDiffuse = fmult(cosAlpha, sp->shader->kDiffuse);
                iaddscaled3(color, scaledDiffuse, &diffuseColor, color);
            }
        }

        // Specular
        if (sp->shader->kSpecular > c_zero) {
            Vec3i scaledColor;
            ireflect3(&light->direction, &hit->normal, &reflect);
            fixed s = idot3(&vertexToEye, &reflect);
            if (s > 0) {
                s = fpow(s, sp->shader->coefficient);
                imult3(&light->color, &sp->shader->specular, &scaledColor);
                iaddscaled3(color, s, &scaledColor, color);
            }
        }

        // Reflected ray
        if (sp->shader->kReflect > c_zero && depth > 0) {
            iRay reflectedRay;
            icopy3(&hit->point, &reflectedRay.point);
            icopy3(&hit->reflect, &reflectedRay.dir);
            // nudge the point to avoid self-intersection
            iaddscaled3(&reflectedRay.point, c_epsilon, &reflectedRay.dir, &reflectedRay.point);
            Hit tmphit;
            init_hit(&tmphit);
            tmphit.object = hit->object; // don't self-intersect
            if (trace(scene, &reflectedRay, &tmphit)) {
                Vec3i reflectColor;
                ivec3(0, 0, 0, &reflectColor);
                shade(scene, &reflectedRay, &tmphit, &reflectColor, depth - 1);
                iaddscaled3(color, sp->shader->kReflect, &reflectColor, color);
            } else {
                iaddscaled3(color, sp->shader->kReflect, &scene->background, color);
            }
        }
    }
}
