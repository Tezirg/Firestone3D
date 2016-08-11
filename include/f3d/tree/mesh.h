#pragma once

#ifndef __F3D_MESH_H
#define __F3D_MESH_H

#include <memory>
#include <cstdint>

#include <assimp/scene.h>
#include <assimp/mesh.h>

namespace f3d {
	namespace tree {
		class Mesh {
		public:
			Mesh();
			Mesh(aiScene *);
			~Mesh();
			
			const aiMatrix4x4&				getTransformation() const;
			void							setTransformation(const aiMatrix4x4& val);
			aiMatrix3x3						getRotation() const;
			void							rotate(const aiMatrix3x3& val);
			aiVector3D						getTranslation() const;
			void							translate(const aiVector3D& val);
			aiVector3D						getScale() const;
			void							scale(const aiVector3D& val);
		public:
			static Mesh*					loadFromFile(std::string& path);
			static void						deepCopy(Mesh* dst, Mesh* src);
			static void						shallowCopy(Mesh *dst, Mesh *src);
		private:
			std::shared_ptr<aiScene>		_ai_scene;
			std::unique_ptr<aiMatrix4x4>	_ai_mat4; //Model space transformation
		};
	}
}

#endif