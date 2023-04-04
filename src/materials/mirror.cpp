#include "mirror.h"
#include "../core/gl.h"

vec3 MirrorMaterial::sample(const vec3 &wi, const vec3 &normal) {
	return reflect(normal, wi);
}

float MirrorMaterial::pdf(const vec3 &wi, const vec3 &wo, const vec3 &normal) {
	return 1;
}

vec3 MirrorMaterial::eval(const vec3 &wi, const vec3 &wo, const vec3 &normal) {
	float c = std::max(dot(wo, normal), 0.001f);
	return vec3(1.f / c);
}