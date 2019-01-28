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

static void updateSphereHit(iSphere* object, iRay* ray, iHit* hit) {
    iaddscaled3(&ray->point, hit->t, &ray->dir, &hit->point);
    sp_normal(object, ray, hit->t, &hit->point, &hit->normal);
    ireflect3(&ray->dir, &hit->normal, &hit->reflect);
}

static void updateTriangleHit(iTriangle* object, iRay* ray, iHit* hit) {
    iaddscaled3(&ray->point, hit->t, &ray->dir, &hit->point);
    tri_normal(object, ray, hit->t, &hit->point, &hit->normal);
    ireflect3(&ray->dir, &hit->normal, &hit->reflect);
}

int trace(Scene* scene, iRay* ray, iHit* hit) {
    int hitCount = 0;
    // Find the nearest object
    hit->object = 0;
    for (int i = 0; i < scene->nSphere; i++) {
        iSphere* sphere = &scene->spheres[i];
        if (hit->object != (void*) sphere && sp_isect(sphere, ray, &hit->t)) {
            hit->object = (void*) sphere;
            hit->type = TYPE_SPHERE;
            hitCount++;
        }
    }
    for (int i = 0; i < scene->nSphere; i++) {
        iTriangle* triangle = &scene->triangles[i];
        if (hit->object != (void*) triangle && tri_isect(triangle, ray, &hit->t)) {
            hit->object = (void*) triangle;
            hit->type = TYPE_TRIANGLE;
            hitCount++;
        }
    }
    return hitCount;
}

void shade(Scene* scene, iRay* ray, iHit* hit, Vec3i* color, uint8_t depth) {
    Vec3i vertexToEye;
    iPhong* shader = 0;

    if (hit->type == TYPE_SPHERE) {
        iSphere* sp = ((iSphere*) hit->object);
        shader = sp->shader; // TODO: handle other object types
        updateSphereHit(sp, ray, hit);
    } else if (hit->type == TYPE_TRIANGLE) {
        iTriangle* tri = (iTriangle*) hit->object;
        shader = tri->shader;
        updateTriangleHit(tri, ray, hit);
    } else {
        return; // unsupported
    }

    if (shader->kSpecular > c_zero) {
        isub3(&hit->point, &ray->point, &vertexToEye);
        inormalize3(&vertexToEye);
    }

    // Shade the hit object
    for (int i = 0; i < scene->nLight; i++) {
        Vec3i reflect;
        iLight* light = &scene->lights[i];
        // Diffuse
        if (shader->kDiffuse > c_zero) {
            fixed cosAlpha = idot3(&light->direction, &hit->normal);
            if (cosAlpha > 0) { // ignore back faces
                Vec3i diffuseColor;
                imult3(&light->color, &shader->diffuse, &diffuseColor);
                fixed scaledDiffuse = fmult(cosAlpha, shader->kDiffuse);
                iaddscaled3(color, scaledDiffuse, &diffuseColor, color);
            }
        }

        // Specular
        if (shader->kSpecular > c_zero) {
            Vec3i scaledColor;
            ireflect3(&light->direction, &hit->normal, &reflect);
            fixed s = idot3(&vertexToEye, &reflect);
            if (s > 0) {
                s = fpow(s, shader->coefficient);
                imult3(&light->color, &shader->specular, &scaledColor);
                iaddscaled3(color, s, &scaledColor, color);
            }
        }

        // Reflected ray
        if (shader->kReflect > c_zero && depth > 0) {
            iRay reflectedRay;
            icopy3(&hit->point, &reflectedRay.point);
            icopy3(&hit->reflect, &reflectedRay.dir);
            // nudge the point to avoid self-intersection
            iaddscaled3(&reflectedRay.point, c_epsilon, &reflectedRay.dir, &reflectedRay.point);
            iHit tmphit;
            init_hit(&tmphit);
            tmphit.object = hit->object; // don't self-intersect
            if (trace(scene, &reflectedRay, &tmphit)) {
                Vec3i reflectColor;
                ivec3(0, 0, 0, &reflectColor);
                shade(scene, &reflectedRay, &tmphit, &reflectColor, depth - 1);
                iaddscaled3(color, shader->kReflect, &reflectColor, color);
            } else {
                iaddscaled3(color, shader->kReflect, &scene->background, color);
            }
        }
    }
}
