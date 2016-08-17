#pragma once

#ifndef _F3D_TRANSFORM_H
#define _F3D_TRANSFORM_H

#include <memory>
#include <cstdint>

#include <assimp/types.h>

namespace f3d {
	namespace utils {
		class Transform {
		public:
			Transform();
			~Transform();
			
			Transform(const Transform& oth);
			Transform&	operator=(const Transform& oth);


			aiMatrix4x4						getTransformation() const;
			void							setTransformation(const aiMatrix4x4& val);
			aiMatrix3x3						getRotation() const;
			void							rotate(const aiMatrix3x3& val);
			aiVector3D						getTranslation() const;
			void							translate(const aiVector3D& val);
			aiVector3D						getScale() const;
			void							scale(const aiVector3D& val);

		private:
			std::unique_ptr<aiMatrix4x4>	_ai_mat4; //Model space transformation
		};
	}
}

#endif