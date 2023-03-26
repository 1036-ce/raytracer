#pragma once

#include "../core/material.h"

class MirrorMaterial : public Material {
public:
	MirrorMaterial() = default;
	MirrorMaterial(float roughness, float eta)
		: roughness(roughness), eta(eta) {}

	MaterialType type() override {
		return MaterialType::REFLECT;
	}

	vec3 sample(const vec3& wi, const vec3& normal) override;

	float pdf(const vec3& wi, const vec3& wo, const vec3& normal) override;

	vec3 eval(const vec3& wi, const vec3& wo, const vec3& normal) override;
private:
  	float distributionGGX(const vec3 &n, const vec3 &h, float roughness);
  	float geometrySchlickGGX(float nv, float k);
  	float geometrySmith(const vec3 &n, const vec3 &v, const vec3 &l, float roughtness);
  	float fresnelSchlick(const vec3 &v, const vec3 &n, float eta);
  	vec3 to_world(const vec3 &v, const vec3 &normal);

  	float roughness;
  	float eta;
};