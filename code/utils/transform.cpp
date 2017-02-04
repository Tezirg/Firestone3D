#include "utils/transform.h"

namespace f3d {
	namespace utils {
		Transform::Transform() {
		}

		Transform::~Transform() {
		}

		Transform::Transform(const Transform& oth) {
			_mat4 = oth._mat4;
			_quat = oth._quat;
		}

		Transform&	Transform::operator=(const Transform& oth) {
			_mat4 = oth._mat4;
			_quat = oth._quat;
			return *this;
		}

		glm::mat4&						Transform::getTransformation() {
			return _mat4;
		}
		const glm::mat4&				Transform::getTransformation() const {
			return _mat4;
		}
		void							Transform::setTransformation(const glm::mat4& val) {
			_mat4 = val;
			_quat = glm::quat_cast(val);
		}

		glm::mat3						Transform::getRotationMatrix() {
			return glm::mat3_cast(_quat);
		}
		const glm::mat3					Transform::getRotationMatrix() const {
			return glm::mat3_cast(_quat);
		}
		const glm::quat					Transform::getRotationQuaternion() const {
			return _quat;
		}
		glm::quat						Transform::getRotationQuaternion() {
			return _quat;
		}
		void							Transform::rotate(const glm::mat3& val) {
			glm::quat tmp = glm::quat_cast(val);
			_quat *= tmp;
			_mat4 = _mat4 * glm::mat4_cast(_quat);
		}
		void							Transform::rotate(const glm::quat& val) {
			_quat *= val;
			_mat4 = _mat4 * glm::mat4_cast(_quat);
		}
		void							Transform::rotate(float angle, const glm::vec3& axis) {
			_quat *= glm::angleAxis(angle, axis);
			_mat4 = _mat4 * glm::mat4_cast(glm::angleAxis(angle, axis));
		}

		glm::vec3						Transform::getTranslation() const {
			return _mat4[3];
		}
		void							Transform::translate(const glm::vec3& val) {
			glm::mat4					translate;

			_mat4 = glm::translate(_mat4, val);
		}

		glm::vec3						Transform::getScale() const {
			glm::vec3 scale;
			glm::quat rotation;
			glm::vec3 translation;
			glm::vec3 skew;
			glm::vec4 perspective;
			glm::decompose(_mat4, scale, rotation, translation, skew, perspective);
			return scale;
		}

		void							Transform::scale(const glm::vec3& val) {
			_mat4 = glm::scale(_mat4, val);
		}

	}
}