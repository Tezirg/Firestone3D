#pragma once

#ifndef __F3D_MESH_H
#define __F3D_MESH_H

#include <memory>
#include <cstdint>

#include <assimp/types.h>

#include "f3d.h"

namespace f3d {
	namespace tree {
		class Mesh {
		public:
			Mesh();
			~Mesh();
			
			virtual aiMatrix4x4						getTransformation() const = 0;
			virtual void							setTransformation(const aiMatrix4x4& val) = 0;
			virtual aiMatrix3x3						getRotation() const = 0;
			virtual void							rotate(const aiMatrix3x3& val) = 0;
			virtual aiVector3D						getTranslation() const = 0;
			virtual void							translate(const aiVector3D& val) = 0;
			virtual aiVector3D						getScale() const = 0;
			virtual void							scale(const aiVector3D& val) = 0;
		public:
			static Mesh*					loadFromFile(std::string& path);
		};
	}
}

#endif