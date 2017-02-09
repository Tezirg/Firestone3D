#pragma once

#ifndef __F3D_MATERIAL_H
#define __F3D_MATERIAL_H

#include <memory>
#include <cstdint>
#include <iostream>
#include <map>
#include <list>
#include <glm/glm.hpp>

#include "tree/texture.h"

namespace f3d {
	namespace tree {
		class Material {
		public:
			enum eColorType {
				F3D_COLOR_UNDEFINED = 0,
				F3D_COLOR_AMBIENT = 1,
				F3D_COLOR_DIFFUSE = 2,
				F3D_COLOR_SPECULAR = 3,
				F3D_COLOR_EMMISIVE = 4,
				F3D_COLOR_REFLECTIVE = 5,
				F3D_COLOR_BEGIN_RANGE = F3D_COLOR_UNDEFINED,
				F3D_COLOR_END_RANGE = F3D_COLOR_REFLECTIVE,
				F3D_COLOR_RANGE_SIZE = (F3D_COLOR_END_RANGE - F3D_COLOR_BEGIN_RANGE + 1),
				F3D_COLOR_MAX_ENUM = 0x7FFFFFFF
			};

			Material(const std::string& name);
			virtual ~Material();

			const std::string&		getName() const;
			void					setname(const std::string& value);
			bool					getColor(const eColorType type, glm::vec3& out) const;
			void					setColor(const eColorType type, glm::vec3& in);
			float					getShineness(void) const;
			void					setShineness(const float value);
			const std::list<f3d::tree::Texture>& getTextures(void) const;
		protected:
			std::string							_name;
			float								_shininess;
			std::map<eColorType, glm::vec3>		_colors;
			std::list<f3d::tree::Texture>		_textures;

		};
	}// tree::
}// f3d::

#endif