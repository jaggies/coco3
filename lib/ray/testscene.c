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
#include "testscene.h"

// Until we get malloc() working...
#define MAXSPHERES 10
#define MAXSHADERS MAXSPHERES
#define MAXLIGHTS 3

static iSphere spheres[MAXSPHERES];
static iLight lights[MAXLIGHTS];
static iPhong shaders[MAXSHADERS];
static iCamera camera;
static Scene scene;

Scene* testScene(fixed aspect) {
    scene.camera = &camera;
    scene.spheres = &spheres[0];
    scene.lights = &lights[0];
    scene.nLight = 0;
    scene.nSphere = 0;

    ivec3(c_one / 8, c_one / 6, c_half, &scene.background);

    // shader 0 = red
    int n = 0;
    ivec3(c_one, 0, 0, &shaders[n].diffuse);
    ivec3(c_one, c_one, c_one, &shaders[n].specular);
    shaders[n].kDiffuse = c_half;
    shaders[n].kSpecular = c_half;
    shaders[n].kReflect = c_half;
    shaders[n].coefficient = fromInt(20);
    n++;

    // shader 1 = green
    ivec3(0, c_one, 0, &shaders[n].diffuse);
    ivec3(c_one, c_one, c_one, &shaders[n].specular);
    shaders[n].kDiffuse = c_half;
    shaders[n].kSpecular = c_half;
    shaders[n].kReflect = c_half;
    shaders[n].coefficient = fromInt(20);
    n++;

    // shader 2 = blue
    ivec3(0, 0, c_one, &shaders[n].diffuse);
    ivec3(c_one, c_one, c_one, &shaders[n].specular);
    shaders[n].kDiffuse = c_half;
    shaders[n].kSpecular = c_half;
    shaders[n].kReflect = c_half;
    shaders[n].coefficient = fromInt(20);
    n++;

    sp_create(-c_one, c_zero, c_zero, fromFloat(0.50f), &shaders[0], &spheres[scene.nSphere++]);
    sp_create(c_zero, c_zero, c_zero, fromFloat(0.50f), &shaders[1], &spheres[scene.nSphere++]);
    sp_create(c_one, c_zero, c_zero, fromFloat(0.50f), &shaders[2], &spheres[scene.nSphere++]);

    Vec3i dir, color;
    ivec3(c_one, -c_one, c_one, &dir);
    ivec3(c_one, c_one, c_one, &color);
    light_create(&dir, &color, &lights[scene.nLight++]);

    Vec3i from;
    Vec3i at;
    Vec3i up;
    ivec3(c_zero, c_zero, fromFloat(2.5f), &from);
    ivec3(c_zero, c_zero, c_zero, &at);
    ivec3(c_zero, c_one, c_zero, &up);
    cam_create(&from, &at, &up, fromInt(45), aspect, &camera);
    return &scene;
}

