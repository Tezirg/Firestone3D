#include "tree/light.h"

namespace f3d {
	namespace tree {

		Light::Light()  
		{ 
			// Defaults to directional, white color, from the top
			setAngleInnerCone(0.0);
			setAngleOuterCone(0.0);
			setAttenuationConstant(1.0);
			setAttenuationLinear(0.0);
			setAttenuationQuadratic(0.0);
			setColorAmbient(glm::vec4(1.0f)); // White
			setColorDiffuse(glm::vec4(1.0f)); // White
			setColorSpecular(glm::vec4(1.0f)); // White
			setDirection(glm::vec3(0.0f, 1.0f, 0.0f));// From the top
			setName(std::string("DefaultLight"));
			setType(F3D_LIGHT_DIRECTIONAL); // Directional
		}

		Light::~Light() {
			std::cout << "Destructor: " << __FILE__ << std::endl;
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
		void					Light::setColorAmbient(const glm::vec4& val) { _ambient_color = val; }
		const glm::vec4&		Light::getColorDiffuse() const { return _diffuse_color; }
		void					Light::setColorDiffuse(const glm::vec4& val) { _diffuse_color = val; }
		const glm::vec4&		Light::getColorSpecular() const { return _specular_color; }
		void					Light::setColorSpecular(const glm::vec4& val) { _specular_color = val; }
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