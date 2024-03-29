#include "emitter.h"

vec3 EmitMaterial::sample(const vec3 &wi, const vec3 &normal) {
	float a = random_float(), b = random_float();
	float z = std::fabs(1.f - 2.f * a);
	float r = std::sqrt(1 - z * z);
	float phi = 2 * PI * b;
	vec3 local_ray(r * std::cos(phi), r * std::sin(phi), z);
	return to_world(local_ray, normal);
}

float EmitMaterial::pdf(const vec3 &wi, const vec3 &wo, const vec3 &normal) {
	if (dot(wo, normal) > 0.f)
		return 0.5 / PI;
	return 0;
}

vec3 EmitMaterial::eval(const vec3 &wi, const vec3 &wo, const vec3 &normal) {
	if (dot(wo, normal) > 0) {
		vec3 diffuse = (vec3)(intensity * color);
		return diffuse;
	}
	return vec3(0.f);
}

vec3 EmitMaterial::to_world(const vec3 &v, const vec3 &normal) {
	vec3 B, C;
	if (std::abs(normal.x) > std::abs(normal.y)) {
		float inv_len = 1.f / std::sqrt(normal.x * normal.x + normal.z * normal.z);
		C = vec3(normal.z * inv_len, 0, -normal.x * inv_len);
	}
	else {
		float inv_len = 1.f / std::sqrt(normal.y * normal.y + normal.z * normal.z);
		C = vec3(0, normal.z * inv_len, -normal.y * inv_len);
	}
	B = cross(C, normal);
	return v.x * B + v.y * C + v.z * normal;
}
