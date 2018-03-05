/*
 * checkerboard.h
 *
 *  Created on: Feb 12, 2014
 *      Author: jmiller
 */

#ifndef CHECKERBOARD_H_
#define CHECKERBOARD_H_

#include "shader.h"
#include "vec2.h"
#include "vec3.h"

struct Shader; 

typedef struct CheckerboardShader {
    struct ShaderOps* op;
    struct Shader* odd;
    struct Shader* even;
    Vec2 scale;
    Vec2 bias;
    // private
    struct Shader* target; // which of the above shaders is being evaluated
} CheckerboardShader;

extern struct Shader* createCheckerboardShader(struct Shader* odd, struct Shader* even, Vec2* scale, Vec2* bias);

#endif /* CHECKERBOARD_H_ */
