#pragma once

#ifndef __F3D_LIGHT_H
#define __F3D_LIGHT_H

#include <memory>
#include <cstdint>
#include <iostream>

#include <glm/glm.hpp>

#include "core/platform.h"
#include "core/types.h"

namespace f3d {
	namespace tree {
		class F3D_API Light {
		 public:
			Light();
			virtual ~Light();

			virtual float					getAngleInnerCone() const = 0;
			virtual void					setAngleInnerCone(float val) = 0;
			virtual float					getAngleOuterCone() const = 0;
			virtual void					setAngleOuterCone(float val) = 0;
			virtual float					getAttenuationConstant() const = 0;
			virtual void					setAttenuationConstant(float val) = 0;
			virtual float					getAttenuationLinear() const = 0;
			virtual void					setAttenuationLinear(float val) = 0;
			virtual float					getAttenuationQuadratic() const = 0;
			virtual void					setAttenuationQuadratic(float val) = 0;
			virtual glm::vec3				getColorAmbient() const = 0;
			virtual void					setColorAmbient(const glm::vec3& val) = 0;
			virtual glm::vec3		 		getColorDiffuse() const = 0;
			virtual void					setColorDiffuse(const glm::vec3& val) = 0;
			virtual glm::vec3		 		getColorSpecular() const = 0;
			virtual void					setColorSpecular(const glm::vec3& val) = 0;
			virtual glm::vec3				getDirection() const = 0;
			virtual void					setDirection(const glm::vec3& val) = 0;
			virtual std::string				getName() const = 0;
			virtual void					setName(const std::string& val) = 0;
			virtual glm::vec3				getPosition() const = 0;
			virtual void					setPosition(const glm::vec3& val) = 0;
			virtual eLightType	 			getType() const = 0;
			virtual void			 		setType(eLightType val) = 0;
		};
	}
}

#endif