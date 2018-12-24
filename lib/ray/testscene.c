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

Scene* testScene() {
    scene.camera = &camera;
    scene.spheres = &spheres[0];
    scene.lights = &lights[0];
    scene.nLight = 0;
    scene.nSphere = 0;

    ivec3(c_one, c_one, c_one, &shaders[0].diffuse);
    ivec3(c_one, c_one, c_one, &shaders[0].specular);
    shaders[0].kDiffuse = c_half;
    shaders[0].kSpecular = c_half;
    shaders[0].coefficient = fromInt(20);

    sp_create(c_zero, c_zero, c_zero, fromFloat(0.35f), &shaders[0], &spheres[scene.nSphere++]);

    Vec3i dir, color;
    ivec3(c_one, -c_one, c_one, &dir);
    ivec3(c_one, c_one, c_one, &color);
    light_create(&dir, &color, &lights[scene.nLight++]);

    Vec3i from;
    Vec3i at;
    Vec3i up;
    ivec3(c_zero, c_zero, c_one, &from);
    ivec3(c_zero, c_zero, c_zero, &at);
    ivec3(c_zero, c_one, c_zero, &up);
    cam_create(&from, &at, &up, fromInt(45), fromFloat(1.0f), &camera);
    return &scene;
}

