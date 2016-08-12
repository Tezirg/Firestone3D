#pragma once

#ifndef __F3D_LIGHT_IMPL_H
#define __F3D_LIGHT_IMPL_H

#include <memory>
#include <cstdint>
#include <iostream>

#include <assimp/light.h>

#include "f3d.h"

namespace f3d {
	namespace tree {
		class LightImpl : public Light {
		public:
			LightImpl();
			LightImpl(aiLight *);
			~LightImpl();

			float					getAngleInnerCone() const;
			void					setAngleInnerCone(float val);
			float					getAngleOuterCone() const;
			void					setAngleOuterCone(float val);
			float					getAttenuationConstant() const;
			void					setAttenuationConstant(float val);
			float					getAttenuationLinear() const;
			void					setAttenuationLinear(float val);
			float					getAttenuationQuadratic() const;
			void					setAttenuationQuadratic(float val);
			aiColor3D				getColorAmbient() const;
			void					setColorAmbient(const aiColor3D& val);
			aiColor3D				getColorDiffuse() const;
			void					setColorDiffuse(const aiColor3D& val);
			aiColor3D				getColorSpecular() const;
			void					setColorSpecular(const aiColor3D& val);
			aiVector3D				getDirection() const;
			void					setDirection(const aiVector3D& val);
			std::string				getName() const;
			void					setName(const std::string& val);
			aiVector3D				getPosition() const;
			void					setPosition(const aiVector3D& val);
			eLightType 				getType() const;
			void			 		setType(eLightType val);
		private:
			std::unique_ptr<aiLight>		_ai_light;
		};
	}
}

#endif