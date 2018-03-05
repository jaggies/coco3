/*
 * triangle.h
 *
 *  Created on: Feb 11, 2014
 *      Author: jmiller
 */

#ifndef TRIANGLE_H_
#define TRIANGLE_H_

#include "shape.h"

struct Shader;

typedef struct Triangle {
    ShapeOps* op;
    struct Shader* shader;
    Vec3  point[1];
    Vec3  normal[1];
    Vec3  edge[2];
    Vec2  uv[3];
} Triangle;

extern struct Shape* createTriangle(Vec3* p0, Vec3* p1, Vec3* p2,
        Vec2* uv0, Vec2* uv1, Vec2* uv2, struct Shader* shader);

#endif /* TRIANGLE_H_ */
