#pragma once

#include "global.h"
#include "color.h"

enum class MaterialType {
	DIFFUSE = 1, 
	REFLECT_REFRACT = 2, 
	REFLECT = 4,
	EMITTER = 8,
	MICROFACET = 16
};

class Material {
public:
	Material() = default;

	virtual MaterialType type() = 0;

	virtual vec3 sample(const vec3& wi, const vec3& normal) = 0;

	virtual float pdf(const vec3& wi, const vec3& wo, const vec3& normal) = 0;

	virtual vec3 eval(const vec3& wi, const vec3& wo, const vec3& normal) = 0;
	
	virtual vec3 emit() {
		return vec3();
	}
};