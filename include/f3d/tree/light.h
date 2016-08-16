#pragma once

#ifndef __F3D_LIGHT_H
#define __F3D_LIGHT_H

#include <memory>
#include <cstdint>
#include <iostream>

#include <assimp/vector3.h>
#include <assimp/matrix4x4.h>
#include <assimp/matrix3x3.h>
#include <assimp/types.h>

namespace f3d {
	namespace tree {
		class Light {
		public:
			Light();
			virtual ~Light();

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
			virtual aiColor3D				getColorAmbient() const = 0;
			virtual void					setColorAmbient(const aiColor3D& val) = 0;
			virtual aiColor3D		 		getColorDiffuse() const = 0;
			virtual void					setColorDiffuse(const aiColor3D& val) = 0;
			virtual aiColor3D		 		getColorSpecular() const = 0;
			virtual void					setColorSpecular(const aiColor3D& val) = 0;
			virtual aiVector3D				getDirection() const = 0;
			virtual void					setDirection(const aiVector3D& val) = 0;
			virtual std::string				getName() const = 0;
			virtual void					setName(const std::string& val) = 0;
			virtual aiVector3D				getPosition() const = 0;
			virtual void					setPosition(const aiVector3D& val) = 0;
			virtual eLightType 				getType() const = 0;
			virtual void			 		setType(eLightType val) = 0;
		};
	}
}

#endif