/*
 * phongshader.c
 *
 *  Created on: Feb 10, 2014
 *      Author: jmiller
 */

#include <stdlib.h>
#include "math.h"
#include "phongshader.h"
#include "hit.h"

static const float _threshold = 0.01f; // minimum factor before we'll call pow

static void evaluatePhong(struct Shader* sh, Hit* hit, Vec3* color) {
    PhongShader* shader = (PhongShader*) sh;

    // Ambient
    copy3(&shader->ambient, color);

    if (hit->inShadow) return;

    // Diffuse
    float cosAlpha = dot3(&hit->normal, &hit->lightRay.dir);
    cosAlpha = fabs(cosAlpha);
    if (cosAlpha > 0) {
        addscaled3(color, cosAlpha, &shader->diffuse, color);
    }

    // Specular
    float cosBeta = dot3(&hit->reflect, &hit->lightRay.dir);
    if (cosBeta > _threshold) {
        float ks = pow(cosBeta, shader->exponent);
        addscaled3(color, ks, &shader->specular, color);
    }
}

static float getReflectionAmountPhong(struct Shader* sh) {
    return ((PhongShader*) sh)->reflect;
}

static float getTransmissionAmountPhong(struct Shader* sh) {
    return ((PhongShader*) sh)->transmit;
}

static float getIndexOfRefractionPhong(struct Shader* sh) {
    return ((PhongShader*) sh)->index;
}

static struct ShaderOps _phongOps;

struct Shader* createPhongShader(Vec3* diffuse, Vec3* specular, Vec3* ambient, float exponent, float index,
        float reflect, float transmit) {
    PhongShader* shader = (PhongShader*) malloc(sizeof(PhongShader));
    if (!_phongOps.evaluate) {
        _phongOps.evaluate = evaluatePhong;
        _phongOps.getReflectionAmount = getReflectionAmountPhong;
        _phongOps.getTransmissionAmount = getTransmissionAmountPhong;
        _phongOps.getIndexOfRefraction = getIndexOfRefractionPhong;
    }
    shader->op = &_phongOps;
    copy3(diffuse, &shader->diffuse);
    copy3(specular, &shader->specular);
    copy3(ambient, &shader->ambient);
    shader->exponent = exponent;
    shader->index = index;
    shader->reflect = reflect;
    shader->transmit = transmit;
    return (struct Shader*) shader;
}
