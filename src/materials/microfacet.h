#pragma once

#include "../core/material.h"

class MicorfacetMaterial : public Material {
public:
	MicorfacetMaterial() = default;
	MicorfacetMaterial(const color_t& kd, float rough, float eta_ )
		: Kd(kd), roughness(rough), eta(eta_) {}

	MaterialType type() override {
		return MaterialType::MICROFACET;
	}

	vec3 sample(const vec3& wi, const vec3& normal) override;

	float pdf(const vec3& wi, const vec3& wo, const vec3& normal) override;

	// https://zhuanlan.zhihu.com/p/152226698
	vec3 eval(const vec3& wi, const vec3& wo, const vec3& normal) override;
private:
  	// https://www.yuque.com/gaoshanliushui-mbfny/sst4c5/az43kq#lf572
  	// normal distribution funciton
  	float distributionGGX(const vec3 &n, const vec3 &h, float roughness);

	float geometrySchlickGGX(float nv, float k);

	float geometrySmith(const vec3& n, const vec3& v, const vec3& l, float roughtness);

	// https://zhuanlan.zhihu.com/p/303168568
	float fresnelSchlick(const vec3& h, const vec3& v, float eta);

	vec3 to_world(const vec3 &v, const vec3 &normal);

	float roughness;
	float eta;
	color_t Kd;
};