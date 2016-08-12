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
			setDirection(aiVector3D(0.0f, 1.0f, 0.0f));// From the top
			setName(std::string("DefaultLight"));
			setType(F3D_LIGHT_DIRECTIONAL);
		}
		
		LightImpl::LightImpl(aiLight *light) : _ai_light(light) {
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
		aiColor3D				LightImpl::getColorAmbient() const { return _ai_light->mColorAmbient; }
		void					LightImpl::setColorAmbient(const aiColor3D& val) { _ai_light->mColorAmbient = val;  }
		aiColor3D				LightImpl::getColorDiffuse() const { return _ai_light->mColorDiffuse; }
		void					LightImpl::setColorDiffuse(const aiColor3D& val) { _ai_light->mColorDiffuse = val; }
		aiColor3D				LightImpl::getColorSpecular() const { return _ai_light->mColorSpecular; }
		void					LightImpl::setColorSpecular(const aiColor3D& val) { _ai_light->mColorSpecular = val; }
		aiVector3D				LightImpl::getDirection() const { return _ai_light->mDirection; }
		void					LightImpl::setDirection(const aiVector3D& val) { _ai_light->mDirection = val; }
		std::string				LightImpl::getName() const { return std::string(_ai_light->mName.data); }
		void					LightImpl::setName(const std::string& val) { _ai_light->mName.Set(val); }
		aiVector3D				LightImpl::getPosition() const { return _ai_light->mPosition; }
		void					LightImpl::setPosition(const aiVector3D& val) { _ai_light->mPosition = val; }
		Light::eLightType		LightImpl::getType() const { return static_cast<eLightType>(_ai_light->mType); }
		void			 		LightImpl::setType(eLightType val) { _ai_light->mType = static_cast<aiLightSourceType>(val);  }
	}
}