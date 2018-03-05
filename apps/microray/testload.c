/*
 * main.c
 *
 *  Created on: Feb 9, 2014
 *      Author: jmiller
 */

#include "sphere.h"
#include "triangle.h"
#include "pointlight.h"
#include "hit.h"
#include "phongshader.h"
#include "checkerboardshader.h"
#include "perspectivecamera.h"
#include "world.h"

Camera* makeCamera(int xres, int yres) {
    Vec3 from; vec3(-1, 1, 1, &from);
    Vec3 at; vec3(0, 0, 0, &at);
    Vec3 up; vec3(0, 1, 0, &up);
    float aspect =  (float) xres / yres;
    return (Camera*) createPerspectiveCamera(&from, &at, &up, 45.0f, aspect);
}

World* testLoad(int xres, int yres) {
    World* world = createWorld();
    world->camera = makeCamera(xres, yres);
    Vec3 diffuse, specular, ambient;
    vec3(0.5f,0.5f,0.5f,&specular);
    vec3(0.0f,0.0f,0.0f,&ambient);
    vec3(0.0f,0.0f,0.0f,&diffuse);
    struct Shader* red = createPhongShader(&diffuse,  &specular,  &ambient, 20.0f, 1.1f, 0.5f, 0.5f);
    vec3(0.0f,0.0f,0.0f,&diffuse);
    struct Shader* blu = createPhongShader(&diffuse,  &specular,  &ambient, 20.0f, 1.1f, 0.5f, 0.5f);
    vec3(0.5f,0.5f,0.5f,&diffuse);
    struct Shader* mirror = createPhongShader(&diffuse, &specular, &ambient, 10.0f, 1.4f, 0.5f, 0.0f);
    vec3(1.0f,0.0f,0.0f, &diffuse);
    struct Shader* odd = createPhongShader(&diffuse, &specular, &ambient, 10.0f, 1.4f, 0.5f, 0.0f);
    vec3(0.0f,1.0f,0.0f, &diffuse);
    struct Shader* even = createPhongShader(&diffuse, &specular, &ambient, 10.0f, 1.4f, 0.5f, 0.0f);
    Vec2 scale; vec2(10.0f,10.0f,&scale);
    Vec2 bias; vec2(0.0f,0.0f,&bias);
    struct Shader* checker = createCheckerboardShader(odd, even, &scale, &bias);
    world->shapes[world->nShapes++] = createSphere(0.25f, 0.0f, 0.0f, 0.25f, red);
    world->shapes[world->nShapes++] = createSphere(-0.25f, 0.0f, 0.0f, 0.25f, blu);
    float plane[][3] = { {-1.0f, -0.25f, -1.0f}, {1.0f, -0.25f, -1.0f}, {1.0f, -0.25f, 1.0f}, {-1.0f, -0.25f, 1.0f} };
    float uv[][2] = { {0.0f,0.0f}, {1.0f,0.0f}, {1.0f,1.0f}, {0.0f,1.0f} };
    world->shapes[world->nShapes++] = createTriangle(
            (Vec3*) plane[0], (Vec3*) plane[1], (Vec3*) plane[2],
            (Vec2*) uv[0], (Vec2*) uv[1], (Vec2*) uv[2],
            checker);
    world->shapes[world->nShapes++] = createTriangle(
            (Vec3*) plane[2], (Vec3*) plane[3], (Vec3*) plane[0],
            (Vec2*) uv[2], (Vec2*) uv[3], (Vec2*) uv[0],
            checker);
    Vec3 point, color;
    vec3(5.0f,5.0f,5.0f, &point);
    vec3(1.0f,1.0f,1.0f, &color);
    world->lights[world->nLights++] = createPointLight(&point, &color);
    vec3(0.2f, 0.3f, 0.7f, &world->background);
    return world;
}

