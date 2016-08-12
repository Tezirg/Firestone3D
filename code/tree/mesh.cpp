#include "tree/mesh.h"
#include "mesh_impl.h"

namespace f3d {
	namespace tree{
		Mesh::Mesh() {
		}

		Mesh::~Mesh() {
		}
		
		Mesh*							Mesh::loadFromFile(std::string& path) { return new MeshImpl();  }
	}
}