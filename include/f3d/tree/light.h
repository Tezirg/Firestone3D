#pragma once

#ifndef __F3D_LIGHT_H
#define __F3D_LIGHT_H

#include <memory>
#include <cstdint>
#include <iostream>
#include <assimp/light.h>

namespace f3d {
	namespace tree {
		class Light {
		public:
			Light();
			Light(aiLight *);
			~Light();

			enum eLightType {
				F3D_LIGHT_UNDEFINED = 0,
				F3D_LIGHT_DIRECTIONAL = 1,
				F3D_LIGHT_POINT = 2,
				F3D_LIGHT_SPOT = 3,
				F3D_LIGHT_BEGIN_RANGE = F3D_LIGHT_UNDEFINED,
				F3D_LIGHT_END_RANGE = F3D_LIGHT_SPOT,
				F3D_LIGHT_RANGE_SIZE = (F3D_LIGHT_SPOT - F3D_LIGHT_UNDEFINED + 1),
				F3D_LIGHT_MAX_ENUM = 0x7FFFFFFF
			};

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
			const aiColor3D&		getColorAmbient() const;
			void					setColorAmbient(const aiColor3D& val);
			const aiColor3D&		getColorDiffuse() const;
			void					setColorDiffuse(const aiColor3D& val);
			const aiColor3D&		getColorSpecular() const;
			void					setColorSpecular(const aiColor3D& val);
			const aiVector3D&		getDirection() const;
			void					setDirection(const aiVector3D& val);
			std::string				getName() const;
			void					setName(const std::string& val);
			const aiVector3D&		getPosition() const;
			void					setPosition(const aiVector3D& val);
			eLightType 				getType() const;
			void			 		setType(eLightType val);
		private:
			std::unique_ptr<aiLight>		_ai_light;
		};
	}
}

#endif