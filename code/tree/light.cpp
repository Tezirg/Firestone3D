#include "tree/light.h"

namespace f3d {
	namespace tree {

		Light::Light()  {
		}

		Light::~Light() {
		}

		float					Light::getAngleInnerCone() const { return _inner_cone; }
		void					Light::setAngleInnerCone(float val) { _inner_cone = val; }
		float					Light::getAngleOuterCone() const { return _outer_cone; }
		void					Light::setAngleOuterCone(float val) { _outer_cone = val; }
		float					Light::getAttenuationConstant() const { return _constant; }
		void					Light::setAttenuationConstant(float val) { _constant = val; }
		float					Light::getAttenuationLinear() const { return _linear; }
		void					Light::setAttenuationLinear(float val) { _linear = val; }
		float					Light::getAttenuationQuadratic() const { return _quadratic; }
		void					Light::setAttenuationQuadratic(float val) { _quadratic = val; }
		const glm::vec4&		Light::getColorAmbient() const { return _ambient_color; }
		void					Light::setColorAmbient(const glm::vec3& val) { _ambient_color.r = val.x; _ambient_color.g = val.y; _ambient_color.b = val.z; }
		const glm::vec4&		Light::getColorDiffuse() const { return _diffuse_color; }
		void					Light::setColorDiffuse(const glm::vec3& val) { _diffuse_color.r = val.x; _diffuse_color.g = val.y; _diffuse_color.b = val.z; }
		const glm::vec4&		Light::getColorSpecular() const { return _specular_color; }
		void					Light::setColorSpecular(const glm::vec3& val) { _specular_color.r = val.x; _specular_color.g = val.y; _specular_color.b = val.z; }
		const glm::vec4&		Light::getDirection() const { return _direction; }
		void					Light::setDirection(const glm::vec3& val) { _direction.x = val.x; _direction.y = val.y; _direction.z = val.z; }
		std::string				Light::getName() const { return _name; }
		void					Light::setName(const std::string& val) { _name.assign(val); }
		const glm::vec4&		Light::getPosition() const { return _position; }
		void					Light::setPosition(const glm::vec3& val) { _position.x = val.x; _position.y = val.y; _position.z = val.z; }

		eLightType				Light::getType() const { return _type; }
		void			 		Light::setType(eLightType val) { _type = val; }
	}
}