#include "tree/mesh.h"
#include "mesh_impl.h"

namespace f3d {
	namespace tree{
		Mesh::Mesh() : _ready(false), _vertices_count(0), _normals_count(0), _indices_count(0), _uv_count(0) {
		}

		Mesh::~Mesh() {
		}


		std::string		Mesh::getName() const {
			return _name;
		}

		void			Mesh::setName(const std::string val) {
			_name.assign(val);
		}

		std::string		Mesh::getMaterialName() const {
			return _material_name;
		}

		void			Mesh::setMaterialName(const std::string val) {
			_material_name.assign(val);
		}

		uint32_t		Mesh::numVertices() const {
			return _vertices_count;
		}

		void			Mesh::addVertex(const float x, const float y, const float z) {
			_vertices_count += 1;
			_vertices.push_back(x);
			_vertices.push_back(y);
			_vertices.push_back(z);
			_vertices.push_back(1.0f);
		}

		void			Mesh::addVertex(glm::vec3& v) {
			_vertices_count += 1;
			_vertices.push_back(v.x);
			_vertices.push_back(v.y);
			_vertices.push_back(v.z);
			_vertices.push_back(1.0f);
		}

		uint32_t		Mesh::numNormals() const {
			return _normals_count;
		}
		void			Mesh::addNormal(const float x, const float y, const float z) {
			_normals_count += 1;
			_normals.push_back(x);
			_normals.push_back(y);
			_normals.push_back(z);
			_normals.push_back(1.0f);
		}
		void			Mesh::addNormal(glm::vec3& n) {
			_normals_count += 1;
			_normals.push_back(n.x);
			_normals.push_back(n.y);
			_normals.push_back(n.z);
			_normals.push_back(1.0f);
		}

		uint32_t		Mesh::numIndices() const {
			return _indices_count;
		}

		uint32_t		Mesh::numTriangles() const {
			return _indices_count / 3;
		}
		void			Mesh::addTriangle(uint32_t a, uint32_t b, uint32_t c) {
			_indices_count += 3;
			_indices.push_back(a);
			_indices.push_back(b);
			_indices.push_back(c);
		}

		uint32_t		Mesh::numUV() const {
			return _uv_count;
		}

		void			Mesh::addUV(float u, float v) {
			_uv_count += 1;
			_uvs.push_back(u);
			_uvs.push_back(1.0f - v);
		}

		bool			Mesh::isRenderReady() const {
			return _ready;
		}
	}
}