#include "tree/light.h"

namespace f3d {
	namespace tree{
		Light::Light() : _ai_light(new aiLight) {
			//Defaults to directional, white color
			setAttenuationConstant(1.0);
			setAttenuationLinear(0.0);
			setAttenuationQuadratic(0.0);
			setColorAmbient(aiColor3D(1.0f));
			setColorDiffuse(aiColor3D(1.0f));
			setColorSpecular(aiColor3D(1.0f));
			setDirection(aiVector3D(0.0f, 1.0f, 0.0f));// From the top
			setName(std::string("DefaultLight"));
			setType(F3D_LIGHT_DIRECTIONAL);
		}
		
		Light::Light(aiLight *light) : _ai_light(light) {
		}

		Light::~Light() {
		}

		float					Light::getAngleInnerCone() const	{ return _ai_light->mAngleInnerCone; }
		void					Light::setAngleInnerCone(float val)	{ _ai_light->mAngleInnerCone = val;  }
		float					Light::getAngleOuterCone() const	{ return _ai_light->mAngleOuterCone; }
		void					Light::setAngleOuterCone(float val) { _ai_light->mAngleOuterCone = val; }
		float					Light::getAttenuationConstant() const { return _ai_light->mAttenuationConstant;  }
		void					Light::setAttenuationConstant(float val) { _ai_light->mAttenuationConstant = val; }
		float					Light::getAttenuationLinear() const { return _ai_light->mAttenuationLinear;  }
		void					Light::setAttenuationLinear(float val) { _ai_light->mAttenuationLinear = val; }
		float					Light::getAttenuationQuadratic() const { return _ai_light->mAttenuationQuadratic; }
		void					Light::setAttenuationQuadratic(float val) { _ai_light->mAttenuationQuadratic = val; }
		const aiColor3D&		Light::getColorAmbient() const { return _ai_light->mColorAmbient; }
		void					Light::setColorAmbient(const aiColor3D& val) { _ai_light->mColorAmbient = val;  }
		const aiColor3D&		Light::getColorDiffuse() const { return _ai_light->mColorDiffuse; }
		void					Light::setColorDiffuse(const aiColor3D& val) { _ai_light->mColorDiffuse = val; }
		const aiColor3D&		Light::getColorSpecular() const { return _ai_light->mColorSpecular; }
		void					Light::setColorSpecular(const aiColor3D& val) { _ai_light->mColorSpecular = val; }
		const aiVector3D&		Light::getDirection() const { return _ai_light->mDirection; }
		void					Light::setDirection(const aiVector3D& val) { _ai_light->mDirection = val; }
		std::string				Light::getName() const { return std::string(_ai_light->mName.data); }
		void					Light::setName(const std::string& val) { _ai_light->mName.Set(val); }
		const aiVector3D&		Light::getPosition() const { return _ai_light->mPosition; }
		void					Light::setPosition(const aiVector3D& val) { _ai_light->mPosition = val; }
		Light::eLightType		Light::getType() const { return static_cast<eLightType>(_ai_light->mType); }
		void			 		Light::setType(eLightType val) { _ai_light->mType = static_cast<aiLightSourceType>(val);  }
	}
}