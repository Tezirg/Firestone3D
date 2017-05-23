#pragma once

#ifndef __F3D_TEXTURE_H
#define __F3D_TEXTURE_H

#include <memory>
#include <cstdint>
#include <iostream>

#include "core/plateform.h"

namespace f3d {
	namespace tree {
		class F3D_API Texture {
		public:
			enum eTextureType {
				F3D_TEXTURE_UNDEFINED = 0,
				F3D_TEXTURE_DIFFUSE = 1,
				F3D_TEXTURE_SPECULAR = 2,
				F3D_TEXTURE_AMBIENT = 3,
				F3D_TEXTURE_EMISSIVE = 4,
				F3D_TEXTURE_HEIGHT = 5,
				F3D_TEXTURE_NORMALS = 6,
				F3D_TEXTURE_SHININESS = 7,
				F3D_TEXTURE_OPACITY = 8,
				F3D_TEXTURE_DISPLACEMENT = 9,
				F3D_TEXTURE_LIGHTMAP = 10,
				F3D_TEXTURE_REFLECTION = 11,
				F3D_TEXTURE_BEGIN_RANGE = F3D_TEXTURE_UNDEFINED,
				F3D_TEXTURE_END_RANGE = F3D_TEXTURE_REFLECTION,
				F3D_TEXTURE_RANGE_SIZE = (F3D_TEXTURE_END_RANGE - F3D_TEXTURE_BEGIN_RANGE + 1),
				F3D_TEXTURE_MAX_ENUM = 0x7FFFFFFF
			};
			enum eTextureAddressMode {
				F3D_ADDRESS_UNDEFINED = 0,
				F3D_ADDRESS_REPEAT = 1,
				F3D_ADDRESS_CLAMP_EDGE = 2,
				F3D_ADDRESS_CLAMP_BORDER = 3,
				F3D_ADDRESS_MIRROR_REPEAT = 4,
				F3D_ADDRESS_MIRROR_CLAMP = 5,
				F3D_ADDRESS_BEGIN_RANGE = F3D_ADDRESS_UNDEFINED,
				F3D_ADDRESS_END_RANGE = F3D_ADDRESS_MIRROR_CLAMP,
				F3D_ADDRESS_RANGE_SIZE = (F3D_ADDRESS_END_RANGE - F3D_ADDRESS_BEGIN_RANGE + 1),
				F3D_ADDRESS_MAX_ENUM = 0x7FFFFFFF
			};

			Texture(uint32_t width, uint32_t height, uint32_t mip_levels, eTextureType type, eTextureAddressMode address_mode[3]);
			virtual ~Texture();

			uint32_t					getWidth() const;
			void						setWidth(const uint32_t value);
			uint32_t					getHeight() const;
			void						setHeight(const uint32_t value);
			uint32_t					getMipmapLevel() const;
			void						setMipmapLevel(const uint32_t value);
			eTextureType				getType() const;
			void						setType(const eTextureType& value);
			eTextureAddressMode			getAddressMode() const;
			void						setAddressMode(const eTextureAddressMode& value);
		protected:
			uint32_t					_width;
			uint32_t					_height;
			uint32_t					_mip_levels;

			eTextureType				_type;
			eTextureAddressMode			_address_mode[3];
		};
	}// tree::
}// f3d::

#endif