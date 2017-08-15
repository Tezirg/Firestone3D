#pragma once

#ifndef __F3D_MATERIAL_H
#define __F3D_MATERIAL_H

#include <memory>
#include <cstdint>
#include <iostream>
#include <map>
#include <list>
#include <algorithm>
#include <glm/glm.hpp>

#include "core/platform.h"
#include "tree/texture.h"
#include "core/types.h"

namespace f3d {
	namespace tree {
		class F3D_API Material {
		public:
			Material(const std::string& name);
			virtual ~Material();

			const std::string&		getName() const;
			void					setname(const std::string& value);
			bool					getColor(const eColorType type, glm::vec4& out) const;
			const glm::vec4&		getColor(const eColorType type) const;
			void					setColor(const eColorType type, glm::vec4& in);
			bool					removeColor(const eColorType type);
			ColorTypeFlags			colorFlags() const;

			void					setShading(const eShadingType& type);
			eShadingType			shadingFlags(void) const;

			float					getShininess(void) const;
			void					setShininess(const float value);
			float					getRoughness(void) const;
			void					setRoughness(const float value);

			void					addTexture(f3d::tree::Texture *texture);
			void					removeTexture(f3d::tree::Texture *texture);
			void					removeTexture(TextureTypeFlags type);
			f3d::tree::Texture *	getTexture(const TextureTypeFlags type) const;
			TextureTypeFlags		textureFlags() const;
		protected:
			std::string							_name;
			float								_shininess;
			float								_roughness;
			std::map<eColorType, glm::vec4>		_colors;
			ColorTypeFlags						_color_mask;
			eShadingType						_shading;
			std::map< TextureTypeFlags, f3d::tree::Texture *>	_textures;
			TextureTypeFlags					_texture_mask;
		};
	}// tree::
}// f3d::

#endif