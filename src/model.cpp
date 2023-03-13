#include "model.h"
#include "OBJ_Loader.h"
#include "geometry.h"

Model::Model(const std::string &path, Material *m) {
	material = m;
	objl::Loader loader;
	loader.LoadFile(path);

	assert(loader.LoadedMeshes.size() == 1);
	auto mesh = loader.LoadedMeshes[0];

	vec3 p_min(std::numeric_limits<double>::infinity());
	vec3 p_max(-std::numeric_limits<double>::infinity());

	for (int i = 0; i < mesh.Vertices.size(); i += 3) {
		vec3 verts[3];
		vec3 nrms[3];
		vec2 uv_coords[3];
		for (int j = 0; j < 3; ++j) {
			auto &vertice = mesh.Vertices[i + j];
			// verts[j]  = vec3(vertice.Position.X, vertice.Position.Y, vertice.Position.Z) * 60.f;
			verts[j]  = vec3(vertice.Position.X, vertice.Position.Y, vertice.Position.Z);
			nrms[j]   = vec3(vertice.Normal.X, vertice.Normal.Y, vertice.Normal.Z);
			uv_coords[j] = vec2(vertice.TextureCoordinate.X, vertice.TextureCoordinate.Y);

			p_min = vec3(fmin(p_min.x, verts[j].x), fmin(p_min.y, verts[j].y), fmin(p_min.z, verts[j].z));
			p_max = vec3(fmax(p_max.x, verts[j].x), fmax(p_max.y, verts[j].y), fmax(p_max.z, verts[j].z));
		}
		triangles.emplace_back(verts, nrms, uv_coords);
	}
	bbox = Box(p_min, p_max);

	std::vector<Object*> objs;
	for (auto& tri: triangles) {
		objs.push_back(&tri);
		area += tri.get_area();
	}
	bvh.root = bvh.build(objs);
}

Intersection Model::intersect(const Ray &ray) {
	auto ret = bvh.intersect(ray);
	ret.material = material;
	return ret;
}

bool Model::is_intersect(const Ray &ray) {
	return false;
}

float Model::get_area() {
	return area;
}

bool Model::is_emmision() {
	return material->is_emission();
}

void Model::sample(Intersection &inter, float &pdf) {
	bvh.sample(inter, pdf);
	inter.material = material;
	inter.emit = material->emit;
}
