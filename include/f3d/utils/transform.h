#pragma once

#ifndef _F3D_TRANSFORM_H
#define _F3D_TRANSFORM_H

#include <memory>
#include <cstdint>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>

namespace f3d {
	namespace utils {
		class Transform {
		public:
			Transform();
			~Transform();
			
			Transform(const Transform& oth);
			Transform&	operator=(const Transform& oth);


			glm::mat4&						getTransformation();
			const glm::mat4&				getTransformation() const;
			void							setTransformation(const glm::mat4& val);

			glm::mat3						getRotationMatrix();
			const glm::mat3					getRotationMatrix() const;
			const glm::quat					getRotationQuaternion() const;
			glm::quat						getRotationQuaternion();
			void							rotate(const glm::mat3& val);
			void							rotate(const glm::quat& val);
			void							rotate(float degree, const glm::vec3& axis);

			glm::vec3						getTranslation() const;
			void							translate(const glm::vec3& val);

			glm::vec3						getScale() const;
			void							scale(const glm::vec3& val);

		private:
			glm::quat						_quat;
			glm::mat4						_mat4; //Model space transformation
		};
	}
}

#endif