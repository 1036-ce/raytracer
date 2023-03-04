#include <iostream>
#include "scene.h"
#include "model.h"

int main() {
	Material jade;
	jade.m_type = MaterialType::DIFFUSE_GLOSSY;
	jade.Ka = color_t(0.135, 0.2225, 0.1575);
	jade.Kd = color_t(0.54, 0.89, 0.63);
	// jade.Kd = vec3(0.94, 0.09, 0.03);
	jade.Ks = color_t(0.316228, 0.316228, 0.316228);
	jade.Ns = 0.1;

	Scene scene(1280, 960, radius(90));
	Model bunny("../obj/bunny/bunny.obj");
	bunny.set_material(&jade);
	

	scene.add(&bunny);
	scene.add(Light(vec3(-20, 70, 20), vec3(1)));
	scene.add(Light(vec3(20, 70, 20), vec3(1)));

	scene.render();

	return 0;
}