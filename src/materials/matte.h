#pragma once

#include "../core/material.h"

class MatteMaterial : public Material {
public:
	MatteMaterial() : Kd(color_t()) {}
	MatteMaterial(const color_t& Kd_) : Kd(Kd_) {}

	MaterialType type() override {
		return MaterialType::DIFFUSE;
	}

	vec3 sample(const vec3& wi, const vec3& normal) override;

	float pdf(const vec3 &wi, const vec3 &wo, const vec3 &normal) override;

	vec3 eval(const vec3& wi, const vec3& wo, const vec3& normal) override;

	color_t Kd;
private:
  	vec3 to_world(const vec3 &v, const vec3 &normal);
};