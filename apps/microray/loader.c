#include <stdio.h>
#include <string.h>
#include <coco.h>
#include "vec3.h"
#include "phongshader.h"
#include "checkerboardshader.h"
#include "shape.h"
#include "shader.h"
#include "sphere.h"
#include "triangle.h"
#include "pointlight.h"
#include "perspectivecamera.h"
#include "world.h"

#define MAXNAME 16

enum { COMMENT = 0, SPHERE, TRIANGLE, PHONG, PERSPECTIVECAMERA, POINTLIGHT, CHECKERBOARD, BACKGROUND };
const char *tokens[] = { "#", "sphere", "triangle", "phongshader", "perspectivecamera", "pointlight", "checkerboard", "background" };
const int kTokens = (sizeof(tokens) / sizeof(tokens[0]));
static struct Shader* defaultShader = 0;

static void addLight(World* world, Light* light) {
    if (light) {
        if (world->nLights < MAXLIGHTS) {
            world->lights[world->nLights++] = light;
        } else {
            printf("Too many lights!\n");
        }
    }
}

static void addShape(World* world, Shape* shape) {
    if (shape) {
        if (world->nShapes < MAXSHAPES) {
            world->shapes[world->nShapes++] = shape;
        } else {
            printf("Too many shapes!\n");
        }
    }
}

static struct Shader* getShader(World* world, const char* shaderName) {
    for (int i = 0; i < world->nShaders; i++) {
        if (strcmp(shaderName, world->shaderNames[i]) == 0) {
            return world->shaders[i];
        }
    }
    printf("Shader %s not found\n", shaderName);
    if (!defaultShader) {
        Vec3 diffuse, specular, ambient; 
        vec3(1,1,1,&diffuse);
        vec3(1,1,1,&specular);
        vec3(0,0,0,&ambient);
        defaultShader = createPhongShader(&diffuse, &specular, &ambient, 20.0f, 1.0f, 0.0f, 0.0f);
    }
    return defaultShader; 
}

static void addShader(World* world, const char* shaderName, struct Shader* shader) {
    if (shader) {
        if (world->nShaders < MAXSHADERS) {
            world->shaders[world->nShaders] = shader;
            world->shaderNames[world->nShaders] = shaderName; // allocated in loadPhongShader
            world->nShaders++;
        } else {
            printf("Too many shaders!\n");
        }
    }
}

static Vec3 loadBackground(const char* args) {
    Vec3 color;
    sscanf(args, "%f %f %f", &color.x, &color.y, &color.z);
    return color;
}

static struct Shader* loadPhongShader(const char* args, const char** outname) {
    char name[MAXNAME];
    Vec3 diffuse, specular, ambient;
    float exponent, index, reflect, transmit;
    sscanf(args, "%s  %f %f %f  %f %f %f  %f %f %f  %f %f %f %f", name,
        &diffuse.x, &diffuse.y, &diffuse.z, &specular.x, &specular.y, &specular.z,
        &ambient.x, &ambient.y, &ambient.z, &exponent, &index, &reflect, &transmit);
    *outname = strdup(name);
    return createPhongShader(&diffuse, &specular, &ambient, exponent, index, reflect, transmit);
}

static struct Shader* loadCheckerboardShader(World* world, const char* args, const char** outname) {
    char name[MAXNAME];
    char oddName[MAXNAME];
    char evenName[MAXNAME];
    Vec2 scale, bias;
    sscanf(args, "%s %s %s %f %f %f %f", name, oddName, evenName, &scale.x, &scale.y, &bias.x, &bias.y);
    *outname = strdup(name);
    return createCheckerboardShader(getShader(world, oddName), getShader(world, evenName), &scale, &bias);
}

static Shape* loadSphere(World* world, const char* args) {
    char material[MAXNAME];
    float x, y, z, r;
    sscanf(args, "%s %f %f %f %f", material, &x, &y, &z, &r);
    return createSphere(x, y, z, r, getShader(world, material));
}

static Shape* loadTriangle(World* world, const char* args) {
    char material[MAXNAME];
    Vec3 p1, p2, p3;
    Vec2 uv1, uv2, uv3;
    sscanf(args, "%s %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f ", material, 
        &p1.x, &p1.y, &p1.z,
        &p2.x, &p2.y, &p2.z,
        &p3.x, &p3.y, &p3.z,
        &uv1.x, &uv1.y,
        &uv2.x, &uv2.y,
        &uv3.x, &uv3.y);
    return createTriangle(&p1, &p2, &p3, &uv1, &uv2, &uv3, getShader(world, material));
}

static Camera* loadPerspectiveCamera(const char* args) {
    Vec3 from, at, up;
    float fov, aspect;
    sscanf(args, "%f %f %f %f %f %f %f %f %f %f %f",
        &from.x, &from.y, &from.z, &at.x, &at.y, &at.z, &up.x, &up.y, &up.z, &fov, &aspect);
    // TODO: get aspect from world
    return createPerspectiveCamera(&from, &at, &up, fov, aspect);
}

static Light* loadPointLight(const char* args) {
    Vec3 position, color;
    sscanf(args, "%f %f %f  %f %f %f", &position.x, &position.y, &position.z, &color.x, &color.y, &color.z);
    return createPointLight(&position, &color);
}

World* loadFile(char* fromPath)
{
    int line = 0;
    char buffer[100];
    World* world = createWorld();
    struct FILE *fp = fopen(fromPath, "r");
    if (!fp) {
        return 0;
    }
    while (!feof(fp)) {
        char* ptr;
        char* linestr = fgets(buffer, sizeof(buffer), fp);
        int foundToken = -1; // unknown
        if (!linestr) {
            break;
        }
        line++;
        for (int i = 0; i < kTokens; i++) {
            ptr = linestr;
            const char* token = tokens[i];
            while (*token == *ptr && *token != 0 && *ptr != 0) {
                ptr++;
                token++;
            }
            if (*token == 0) { // found
                foundToken = i;
                break; // token found
            }
        }
        if (foundToken == -1) {
            printf("Bad token, line %d: \n\t%s", line, ptr);
            continue; // ignore rest of line
        } else {
            const char* name;
            struct Shader* shader;
            switch(foundToken) {
                case COMMENT:
                    break; // ignore
                case PERSPECTIVECAMERA:
                    world->camera = loadPerspectiveCamera(ptr);
                    break;
                case SPHERE:
                    addShape(world, loadSphere(world, ptr));
                    break;
                case POINTLIGHT:
                    addLight(world, loadPointLight(ptr));
                    break;
                case PHONG:
                    shader = loadPhongShader(ptr, &name);
                    addShader(world, name, shader);
                    break;
                case BACKGROUND:
                    world->background = loadBackground(ptr);
                    break;
                case TRIANGLE:
                    addShape(world, loadTriangle(world, ptr));
                    break;
                case CHECKERBOARD:
                    shader = loadCheckerboardShader(world, ptr, &name);
                    addShader(world, name, shader);
                    break;
                default:
                    printf("Unimplemented token %s, args:%s", tokens[foundToken], ptr);
            }
        }
    }
    return world;
}
