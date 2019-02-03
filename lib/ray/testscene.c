/*
 * testscene.c
 *
 *  Created on: Dec 24, 2018
 *      Author: jmiller
 */

#include "os.h"
#include "isphere.h"
#include "icamera.h"
#include "ilight.h"
#include "itriangle.h"
#include "testscene.h"

#define MAXSPHERES 10
#define MAXTRIANGLES 4
#define MAXLIGHTS 3

Scene* testScene(fixed aspect) {
    Scene* scene = (Scene*) malloc(sizeof(Scene));
    scene->camera = 0;
    scene->spheres = (iSphere**) malloc(MAXSPHERES * sizeof(iSphere*));
    scene->triangles = (iTriangle**) malloc(MAXTRIANGLES * sizeof(iTriangle*));
    scene->lights = (iLight**) malloc(MAXLIGHTS * sizeof(iLight*));
    scene->nLight = 0;
    scene->nSphere = 0;
    scene->nTriangle = 0;

    ivec3(c_one / 8, c_one / 6, c_half, &scene->background);

    iPhong* red = (iPhong*) malloc(sizeof(iPhong));
    ivec3(c_one, 0, 0, &red->diffuse);
    ivec3(c_one, c_one, c_one, &red->specular);
    red->kDiffuse = c_half;
    red->kSpecular = c_half;
    red->kReflect = c_half;
    red->coefficient = fromInt(20);

    iPhong* green = (iPhong*) malloc(sizeof(iPhong));
    ivec3(0, c_one, 0, &green->diffuse);
    ivec3(c_one, c_one, c_one, &green->specular);
    green->kDiffuse = c_half;
    green->kSpecular = c_half;
    green->kReflect = c_half;
    green->coefficient = fromInt(20);

    iPhong* blue = (iPhong*) malloc(sizeof(iPhong));
    ivec3(0, 0, c_one, &blue->diffuse);
    ivec3(c_one, c_one, c_one, &blue->specular);
    blue->kDiffuse = c_half;
    blue->kSpecular = c_half;
    blue->kReflect = c_half;
    blue->coefficient = fromInt(20);

    iPhong* mirror = (iPhong*) malloc(sizeof(iPhong));
    ivec3(c_one, c_one, c_one, &mirror->diffuse);
    ivec3(c_one, c_one, c_one, &mirror->specular);
    mirror->kDiffuse = c_half;
    mirror->kSpecular = c_half;
    mirror->kReflect = c_half;
    mirror->coefficient = fromInt(20);

    scene->spheres[scene->nSphere++] = sp_create(-c_one, c_zero, c_zero, fromFloat(0.50f), red);
    scene->spheres[scene->nSphere++] = sp_create(c_zero, c_zero, c_zero, fromFloat(0.50f), green);
    scene->spheres[scene->nSphere++] = sp_create(c_one, c_zero, c_zero, fromFloat(0.50f), blue);

    Vec3i p[4];
    ivec3(-c_one, -c_half, -c_one, &p[0]);
    ivec3( c_one, -c_half, -c_one, &p[1]);
    ivec3( c_one, -c_half,  c_one, &p[2]);
    ivec3(-c_one, -c_half,  c_one, &p[3]);
    scene->triangles[scene->nTriangle++] = tri_create(&p[0], &p[1], &p[2], mirror);
    scene->triangles[scene->nTriangle++] = tri_create(&p[0], &p[2], &p[3], mirror);

    Vec3i dir, color;
    ivec3(c_one, c_one, c_one, &dir);
    ivec3(c_one, c_one, c_one, &color);
    scene->lights[scene->nLight++] = light_create(&dir, &color);

    Vec3i from;
    Vec3i at;
    Vec3i up;
    ivec3(c_zero, c_zero, fromFloat(2.5f), &from);
    ivec3(c_zero, c_zero, c_zero, &at);
    ivec3(c_zero, c_one, c_zero, &up);
    scene->camera = cam_create(&from, &at, &up, fromInt(45), aspect);
    return scene;
}

