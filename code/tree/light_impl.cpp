#include "light_impl.h"

namespace f3d {
	namespace tree{
		LightImpl::LightImpl() : _ai_light(new aiLight) {
			//Defaults to directional, white color
			setAttenuationConstant(1.0);
			setAttenuationLinear(0.0);
			setAttenuationQuadratic(0.0);
			setColorAmbient(aiColor3D(1.0f));
			setColorDiffuse(aiColor3D(1.0f));
			setColorSpecular(aiColor3D(1.0f));
			setDirection(glm::vec3(0.0f, 1.0f, 0.0f));// From the top
			setName(std::string("DefaultLight"));
			setType(F3D_LIGHT_DIRECTIONAL);
		}
		
		LightImpl::LightImpl(aiLight *light) : _ai_light(new aiLight(*light)) {
		}

		LightImpl::~LightImpl() {
		}

		float					LightImpl::getAngleInnerCone() const	{ return _ai_light->mAngleInnerCone; }
		void					LightImpl::setAngleInnerCone(float val)	{ _ai_light->mAngleInnerCone = val;  }
		float					LightImpl::getAngleOuterCone() const	{ return _ai_light->mAngleOuterCone; }
		void					LightImpl::setAngleOuterCone(float val) { _ai_light->mAngleOuterCone = val; }
		float					LightImpl::getAttenuationConstant() const { return _ai_light->mAttenuationConstant;  }
		void					LightImpl::setAttenuationConstant(float val) { _ai_light->mAttenuationConstant = val; }
		float					LightImpl::getAttenuationLinear() const { return _ai_light->mAttenuationLinear;  }
		void					LightImpl::setAttenuationLinear(float val) { _ai_light->mAttenuationLinear = val; }
		float					LightImpl::getAttenuationQuadratic() const { return _ai_light->mAttenuationQuadratic; }
		void					LightImpl::setAttenuationQuadratic(float val) { _ai_light->mAttenuationQuadratic = val; }
		glm::vec3				LightImpl::getColorAmbient() const { return glm::vec3(_ai_light->mColorAmbient.r, _ai_light->mColorAmbient.g, _ai_light->mColorAmbient.b); }
		void					LightImpl::setColorAmbient(const aiColor3D& val) { _ai_light->mColorAmbient = val;  }
		glm::vec3				LightImpl::getColorDiffuse() const { return glm::vec3(_ai_light->mColorDiffuse.r, _ai_light->mColorDiffuse.g, _ai_light->mColorDiffuse.b); }
		void					LightImpl::setColorDiffuse(const aiColor3D& val) { _ai_light->mColorDiffuse = val; }
		glm::vec3				LightImpl::getColorSpecular() const { return glm::vec3(_ai_light->mColorSpecular.r, _ai_light->mColorSpecular.g, _ai_light->mColorSpecular.b); }
		void					LightImpl::setColorSpecular(const aiColor3D& val) { _ai_light->mColorSpecular = val; }
		glm::vec3				LightImpl::getDirection() const { return glm::vec3(_ai_light->mDirection.x, _ai_light->mDirection.y, _ai_light->mDirection.z); }
		void					LightImpl::setDirection(const glm::vec3& val) { 
			_ai_light->mDirection.x = val.x;
			_ai_light->mDirection.y = val.y ; 
			_ai_light->mDirection.z = val.z;
		}
		std::string				LightImpl::getName() const { return std::string(_ai_light->mName.data); }
		void					LightImpl::setName(const std::string& val) { _ai_light->mName.Set(val); }
		glm::vec3				LightImpl::getPosition() const { return glm::vec3(_ai_light->mPosition.x, _ai_light->mPosition.y, _ai_light->mPosition.z); }
		void					LightImpl::setPosition(const glm::vec3& val) {
			_ai_light->mPosition.x = val.x; 
			_ai_light->mPosition.y = val.y;
			_ai_light->mPosition.z = val.z;
		}
		Light::eLightType		LightImpl::getType() const { return static_cast<eLightType>(_ai_light->mType); }
		void			 		LightImpl::setType(eLightType val) { _ai_light->mType = static_cast<aiLightSourceType>(val);  }
	}
}