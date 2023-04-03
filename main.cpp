#include <iostream>
#include <chrono>
#include "scene.h"
#include "model.h"
#include "matte.h"
#include "emitter.h"
#include "microfacet.h"
#include "sphere.h"
#include "mirror.h"

int main() {
    // Change the definition here to change resolution
    Scene scene(784, 784, radian(40));

    Material *red = new MatteMaterial(color_t(0.63f, 0.065f, 0.05f));
    Material *green = new MatteMaterial(color_t(0.14f, 0.45f, 0.091f));
    Material *white = new MatteMaterial(color_t(0.725f, 0.71f, 0.68f));
    Material *light = new EmitMaterial(color_t(1, 1, 1), vec3(47));
    Material *ball = new MicorfacetMaterial(color_t(0.73f, 0.25f, 0.35f), 0.4f, 1.85);
    // Material *ball = new MatteMaterial(color_t(0.73f, 0.25f, 0.35f));
    Material *mirror = new MirrorMaterial(0.1, 1.85);

    Model floor("../models/cornellbox/floor.obj", white);
    Model shortbox("../models/cornellbox/shortbox.obj", white);
    Model tallbox("../models/cornellbox/tallbox.obj", white);
    Model left("../models/cornellbox/left.obj", red);
    Model right("../models/cornellbox/right.obj", green);
    Model light_("../models/cornellbox/light.obj", light);
    Sphere sp(Point3(160, 100, 330), 100, ball);

    scene.add(&floor);
    // scene.add(&shortbox);
    scene.add(&tallbox);
    scene.add(&left);
    scene.add(&right);
    scene.add(&light_);
    scene.add(&sp);
    // scene.add(&bunny);



    auto start = std::chrono::system_clock::now();
	scene.render();
    auto stop = std::chrono::system_clock::now();

    std::cout << "Render complete: \n";
    std::cout << "Time taken: " << std::chrono::duration_cast<std::chrono::hours>(stop - start).count() << " hours\n";
    std::cout << "          : " << std::chrono::duration_cast<std::chrono::minutes>(stop - start).count() % 60<< " minutes\n";
    std::cout << "          : " << std::chrono::duration_cast<std::chrono::seconds>(stop - start).count() % 60 << " seconds\n";

    return 0;
}
