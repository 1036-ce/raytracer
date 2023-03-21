#pragma once

#include "../core/material.h"

class EmitMaterial : public Material {
public:
	EmitMaterial() : color(color_t()), intensity(vec3()) {}
	EmitMaterial(const color_t& c, const vec3& ins)
		: color(c), intensity(ins) {}

	MaterialType type() override {
		return MaterialType::EMITTER;
	}

	vec3 sample(const vec3& wi, const vec3& normal) override;

	float pdf(const vec3& wi, const vec3& wo, const vec3& normal) override;

	vec3 eval(const vec3& wi, const vec3& wo, const vec3& normal) override;

	vec3 emit() override {
		return vec3(intensity * color);
	}

private:
	color_t color;
	vec3 intensity;

  	vec3 to_world(const vec3 &v, const vec3 &normal);
};