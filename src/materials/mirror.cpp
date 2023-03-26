#include "mirror.h"
#include "../core/gl.h"

vec3 MirrorMaterial::sample(const vec3 &wi, const vec3 &normal) {
	float a = random_float(), b = random_float();
	float z = std::fabs(1.f - 2.f * a);
	float r = std::sqrt(1 - z * z);
	float phi = 2 * PI * b;
	vec3 local_ray(r * std::cos(phi), r * std::sin(phi), z);
	return to_world(local_ray, normal);
}

float MirrorMaterial::pdf(const vec3 &wi, const vec3 &wo, const vec3 &normal) {
	if (dot(wo, normal) > 0.f)
		return 0.5 / PI;
	return 0;
}

vec3 MirrorMaterial::eval(const vec3 &wi, const vec3 &wo, const vec3 &normal) {
	if (dot(wo, normal) <= 0)
		return vec3(0);
	
	vec3 l = -wi, v = wo, n = normal;
	vec3 r = reflect(n, wi);


	float F = fresnelSchlick(r, n, this->eta);
	return vec3(F / dot(v, n));
}

float MirrorMaterial::distributionGGX(const vec3 &n, const vec3 &h, float roughness) {
	float a = roughness * roughness, a2 = a * a;
	float nh = dot(n, h);
	float div = nh * nh * (a2 - 1) + 1;
	div = PI * div * div;
	return a2 / std::max(div, 0.00001f);
}

float MirrorMaterial::geometrySchlickGGX(float nv, float k) {
	float div = nv / (1.0 - k) + k;
	return nv / div;
}

float MirrorMaterial::geometrySmith(const vec3 &n, const vec3 &v, const vec3 &l, float roughtness) {
	float r = roughtness + 1.f;
	float k = r * r * 0.125f;
	float nv = std::max(dot(n, v), 0.f);
	float nl = std::max(dot(n, l), 0.f);
	float ggx1 = geometrySchlickGGX(nv, k);
	float ggx2 = geometrySchlickGGX(nl, k);
	return ggx1 * ggx2;
}

float MirrorMaterial::fresnelSchlick(const vec3 &v, const vec3 &n, float eta) {
	float f0 = (eta - 1.f) / (eta + 1.f);
	f0 = f0 * f0;
	float c = 1 - dot(v, n);
	float x2 = c * c;
	float x5 = x2 * x2 * c;
	return f0 + (1 - f0) * x5;
}

vec3 MirrorMaterial::to_world(const vec3 &v, const vec3 &normal) {
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
