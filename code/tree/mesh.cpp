#include "tree/mesh.h"

namespace f3d {
	namespace tree{
		Mesh::Mesh() : _ai_scene(new aiScene), _ai_mat4(new aiMatrix4x4) {
		}

		Mesh::Mesh(aiScene *scene) : _ai_scene(scene), _ai_mat4(new aiMatrix4x4) {
		}

		Mesh::~Mesh() {
		}

		const aiMatrix4x4&				Mesh::getTransformation() const { return *_ai_mat4; }

		void							Mesh::setTransformation(const aiMatrix4x4& val) { 
			_ai_mat4->a1 = val.a1; _ai_mat4->a2 = val.a2; _ai_mat4->a3 = val.a3; _ai_mat4->a4 = val.a4; 
			_ai_mat4->b1 = val.b1; _ai_mat4->b2 = val.b2; _ai_mat4->b3 = val.b3; _ai_mat4->b4 = val.b4; 
			_ai_mat4->c1 = val.c1; _ai_mat4->c2 = val.c2; _ai_mat4->c3 = val.c3; _ai_mat4->c4 = val.c4; 
			_ai_mat4->d1 = val.d1; _ai_mat4->d2 = val.d2; _ai_mat4->d3 = val.d3; _ai_mat4->d4 = val.d4; 
		}

		aiMatrix3x3				Mesh::getRotation() const {
			aiVector3D					t;
			aiQuaternion				r;

			_ai_mat4->DecomposeNoScaling(r, t);
			(void)t;
			return r.GetMatrix();
		}

		void							Mesh::rotate(const aiMatrix3x3& val) {
			(*_ai_mat4) *= aiMatrix4x4(val);
		}

		aiVector3D				Mesh::getTranslation() const {
			return aiVector3D(_ai_mat4->a4, _ai_mat4->b4, _ai_mat4->c4);
		}

		void							Mesh::translate(const aiVector3D& val) {
			aiMatrix4x4					translate;

			aiMatrix4x4::Translation(val, translate);
			(*_ai_mat4) *= translate;
		}

		aiVector3D				Mesh::getScale() const {
			aiVector3D					t, s;
			aiQuaternion				r;

			_ai_mat4->Decompose(s, r, t);
			(void)t;
			(void)r;
			return s;
		}

		void							Mesh::scale(const aiVector3D& val) {
			aiMatrix4x4					scaling;
			
			aiMatrix4x4::Scaling(val, scaling);
			(*_ai_mat4) *= scaling;
		}
		
		Mesh*							Mesh::loadFromFile(std::string& path) { return nullptr;  }
		void							Mesh::deepCopy(Mesh* dst, Mesh* src) {}
		void							Mesh::shallowCopy(Mesh *dst, Mesh *src) {}
	}
}