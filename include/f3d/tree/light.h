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

			virtual float					getSpotExponent() const;
			virtual void					setSpotExponent(float val);
			virtual float					getSpotCutoff() const;
			virtual void					setSpotCutoff(float val);
			virtual float					getAttenuationConstant() const;
			virtual void					setAttenuationConstant(float val);
			virtual float					getAttenuationLinear() const;
			virtual void					setAttenuationLinear(float val);
			virtual float					getAttenuationQuadratic() const;
			virtual void					setAttenuationQuadratic(float val);
			virtual const glm::vec4&		getColorAmbient() const;
			virtual void					setColorAmbient(const glm::vec4& val);
			virtual const glm::vec4& 		getColorDiffuse() const;
			virtual void					setColorDiffuse(const glm::vec4& val);
			virtual const glm::vec4& 		getColorSpecular() const;
			virtual void					setColorSpecular(const glm::vec4& val);
			virtual const glm::vec4&		getDirection() const;
			virtual void					setDirection(const glm::vec3& val);
			virtual std::string				getName() const;
			virtual void					setName(const std::string& val);
			virtual const glm::vec4&		getPosition() const;
			virtual void					setPosition(const glm::vec3& val);
			virtual eLightType	 			getType() const;
			virtual void			 		setType(eLightType val);
		protected:
			std::string									_name;
			eLightType									_type;
			glm::vec4									_position;
			glm::vec4									_direction;
			glm::vec4									_ambient_color;
			glm::vec4									_diffuse_color;
			glm::vec4									_specular_color;
			float										_spot_exponent;
			float										_spot_cutoff;
			float										_spot_cos_cutoff;
			float										_constant;
			float										_linear;
			float										_quadratic;
		};
	}
}

#endif