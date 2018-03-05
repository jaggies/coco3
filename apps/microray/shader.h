/*
 * shader.h
 *
 *  Created on: Feb 10, 2014
 *      Author: jmiller
 */

#ifndef SHADER_H_
#define SHADER_H_

struct Hit;
struct Vec3;

struct ShaderOps {
    void (*evaluate)(struct Shader*, struct Hit* hit, struct Vec3* color);
    float (*getReflectionAmount)(struct Shader* shader);
    float (*getTransmissionAmount)(struct Shader* shader);
    float (*getIndexOfRefraction)(struct Shader* shader);
};

struct Shader {
	struct ShaderOps* op; // these must be first in all "inherited" classes
};

#endif /* SHADER_H_ */
