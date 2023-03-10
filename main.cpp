#include <iostream>
#include <chrono>
#include "scene.h"
#include "model.h"

int main() {
	Material jade;
	jade.m_type = MaterialType::DIFFUSE_GLOSSY;
	jade.Ka = color_t(0.135, 0.2225, 0.1575);
	jade.Kd = color_t(0.5, 0.5, 0.5);
	jade.Ks = color_t(0.3, 0.3, 0.3);
	jade.Ni = 1.5;
	jade.Ns = 0.1;

	Scene scene(1280, 960, radius(90));
	Model bunny("../obj/african_head/african_head.obj");
	bunny.set_material(&jade);
	

	scene.add(&bunny);
	scene.add(Light(vec3(-20, 70, 20), vec3(2100)));
	scene.add(Light(vec3(20, 70, 20), vec3(2000)));

	std::cout << "Render start" << std::endl;
	auto start = std::chrono::system_clock::now();

	scene.render();

	auto stop  = std::chrono::system_clock::now();
	std::cout << "Render complete: \n"; 
	std::cout << "Time token: " << std::chrono::duration_cast<std::chrono::hours>(stop - start).count() << " hours\n";
	std::cout << "          : " << std::chrono::duration_cast<std::chrono::minutes>(stop - start).count() << " minutes\n";
	std::cout << "          : " << std::chrono::duration_cast<std::chrono::seconds>(stop - start).count() << " seconds\n";


	return 0;
}