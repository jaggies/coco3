/*
 * checkerboardshader.c
 *
 *  Created on: Feb 12, 2014
 *      Author: jmiller
 */

#include <stdlib.h>
#include "hit.h"
#include "checkerboardshader.h"

static void evaluateCheckerboard(struct Shader* sh, Hit* hit, Vec3* color)
{
    CheckerboardShader* shader = (CheckerboardShader*) sh;
    Vec2 p; multadd2(&shader->scale, &hit->uv, &shader->bias, &p);
    int u = ((int) floorf(p.x)) & 1;
    int v = ((int) floorf(p.y)) & 1;
    shader->target = (u^v) ? shader->odd : shader->even;
    shader->target->op->evaluate(shader->target, hit, color);
}

static float getReflectionAmountChecker(struct Shader* sh) {
    CheckerboardShader* shader = (CheckerboardShader*) sh;
    return shader->target->op->getReflectionAmount(shader->target);
}

static float getTransmissionAmountChecker(struct Shader* sh) {
    CheckerboardShader* shader = (CheckerboardShader*) sh;
    return shader->target->op->getTransmissionAmount(shader->target);
}

static float getIndexOfRefractionChecker(struct Shader* sh) {
    CheckerboardShader* shader = (CheckerboardShader*) sh;
    return shader->target->op->getIndexOfRefraction(shader->target);
}

static struct ShaderOps _checkerOps;

struct Shader* createCheckerboardShader(struct Shader* odd, struct Shader* even, Vec2* scale, Vec2* bias)  {
    CheckerboardShader* shader = (CheckerboardShader*) malloc(sizeof(CheckerboardShader));
    if (!_checkerOps.evaluate) {
        _checkerOps.evaluate = evaluateCheckerboard;
        _checkerOps.getIndexOfRefraction = getIndexOfRefractionChecker;
        _checkerOps.getReflectionAmount = getReflectionAmountChecker;
        _checkerOps.getTransmissionAmount = getTransmissionAmountChecker;
    }
    shader->op = &_checkerOps;
    shader->even = even;
    shader->odd = odd;
    copy2(scale, &shader->scale);
    copy2(bias, &shader->bias);
    return (struct Shader*) shader;
}



