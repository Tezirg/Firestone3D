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
			glm::vec3				getColorAmbient() const;
			void					setColorAmbient(const aiColor3D& val);
			glm::vec3				getColorDiffuse() const;
			void					setColorDiffuse(const aiColor3D& val);
			glm::vec3				getColorSpecular() const;
			void					setColorSpecular(const aiColor3D& val);
			glm::vec3				getDirection() const;
			void					setDirection(const glm::vec3& val);
			std::string				getName() const;
			void					setName(const std::string& val);
			glm::vec3				getPosition() const;
			void					setPosition(const glm::vec3& val);
			eLightType 				getType() const;
			void			 		setType(eLightType val);
		private:
			std::unique_ptr<aiLight>		_ai_light;
		};
	}
}

#endif