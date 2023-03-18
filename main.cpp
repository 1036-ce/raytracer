#include <iostream>
#include <chrono>
#include "scene.h"
#include "model.h"
#include "matte.h"
#include "emitter.h"

int main() {
    // Change the definition here to change resolution
    Scene scene(784, 784, radius(40));

    // Material* red = new Material(MaterialType::DIFFUSE, vec3(0.0f));
    // red->Kd = vec3(0.63f, 0.065f, 0.05f);
    // Material* green = new Material(MaterialType::DIFFUSE, vec3(0.0f));
    // green->Kd = vec3(0.14f, 0.45f, 0.091f);
    // Material* white = new Material(MaterialType::DIFFUSE, vec3(0.0f));
    // white->Kd = vec3(0.725f, 0.71f, 0.68f);
    // Material* light = new Material(MaterialType::DIFFUSE, 
    //                                   (8.0f * vec3(0.747f+0.058f, 0.747f+0.258f, 0.747f) 
    //                                 + 15.6f * vec3(0.740f+0.287f,0.740f+0.160f,0.740f) 
    //                                 + 18.4f *vec3(0.737f+0.642f,0.737f+0.159f,0.737f)));
    // light->Kd = vec3(0.65f);

    Material *red = new MatteMaterial(color_t(0.63f, 0.065f, 0.05f));
    Material *green = new MatteMaterial(color_t(0.14f, 0.45f, 0.091f));
    Material *white = new MatteMaterial(color_t(0.725f, 0.71f, 0.68f));
    Material *light = new EmitMaterial(color_t(1, 1, 1), vec3(42, 42, 42));

    Model floor("../models/cornellbox/floor.obj", white);
    Model shortbox("../models/cornellbox/shortbox.obj", white);
    Model tallbox("../models/cornellbox/tallbox.obj", white);
    Model left("../models/cornellbox/left.obj", red);
    Model right("../models/cornellbox/right.obj", green);
    Model light_("../models/cornellbox/light.obj", light);

    scene.add(&floor);
    scene.add(&shortbox);
    scene.add(&tallbox);
    scene.add(&left);
    scene.add(&right);
    scene.add(&light_);



    auto start = std::chrono::system_clock::now();
	scene.render();
    auto stop = std::chrono::system_clock::now();

    std::cout << "Render complete: \n";
    std::cout << "Time taken: " << std::chrono::duration_cast<std::chrono::hours>(stop - start).count() << " hours\n";
    std::cout << "          : " << std::chrono::duration_cast<std::chrono::minutes>(stop - start).count() % 60<< " minutes\n";
    std::cout << "          : " << std::chrono::duration_cast<std::chrono::seconds>(stop - start).count() % 60 << " seconds\n";

    return 0;
}