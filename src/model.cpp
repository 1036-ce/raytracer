#include "model.h"
#include "OBJ_Loader.h"
#include "geometry.h"

Model::Model(const std::string &path) {
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
			verts[j]  = vec3(vertice.Position.X, vertice.Position.Y, vertice.Position.Z) * 60.f;
			nrms[j]   = vec3(vertice.Normal.X, vertice.Normal.Y, vertice.Normal.Z);
			uv_coords[j] = vec2(vertice.TextureCoordinate.X, vertice.TextureCoordinate.Y);

			p_min = vec3(fmin(p_min.x, verts[j].x), fmin(p_min.y, verts[j].y), fmin(p_min.z, verts[j].z));
			p_max = vec3(fmax(p_max.x, verts[j].x), fmax(p_max.y, verts[j].y), fmax(p_max.z, verts[j].z));
		}
		triangles.emplace_back(verts, nrms, uv_coords);
	}
	bbox = Box(p_min, p_max);

	std::vector<Object*> objs;
	for (auto& tri: triangles)
		objs.push_back(&tri);
	bvh.root = bvh.build(objs);
}

Intersection Model::intersect(const Ray &ray) {
	auto ret = bvh.intersect(ray);
	ret.material = material;
	return ret;
}
