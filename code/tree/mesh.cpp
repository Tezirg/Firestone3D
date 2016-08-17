#include "tree/mesh.h"
#include "mesh_impl.h"

namespace f3d {
	namespace tree{
		Mesh::Mesh() {
		}

		Mesh::~Mesh() {
		}


		std::string		Mesh::getName() const {
			return _name;
		}

		void			Mesh::setName(const std::string val) {
			_name.assign(val);
		}

		uint32_t		Mesh::numVertices() const {
			return _vertices.size() / 3;
		}

		void			Mesh::addVertex(const float x, const float y, const float z) {
			_vertices.push_back(x);
			_vertices.push_back(y);
			_vertices.push_back(z);
		}

		void			Mesh::addVertex(aiVector3D& v) {
			_vertices.push_back(v.x);
			_vertices.push_back(v.y);
			_vertices.push_back(v.z);
		}

		uint32_t		Mesh::numNormals() const {
			return _normals.size() / 3;
		}
		void			Mesh::addNormal(const float x, const float y, const float z) {
			_normals.push_back(x);
			_normals.push_back(y);
			_normals.push_back(z);
		}
		void			Mesh::addNormal(aiVector3D& n) {
			_normals.push_back(n.x);
			_normals.push_back(n.y);
			_normals.push_back(n.z);
		}

		uint32_t		Mesh::numTriangles() const {
			return _indices.size() / 3;
		}
		void			Mesh::addTriangle(uint32_t a, uint32_t b, uint32_t c) {
			_indices.push_back(a);
			_indices.push_back(b);
			_indices.push_back(c);
		}

		uint32_t		Mesh::numUV() const {
			return _uvs.size() / 2;
		}

		void			Mesh::addUV(float u, float v) {
			_uvs.push_back(u);
			_uvs.push_back(v);
		}
	}
}