#pragma once

#ifndef __F3D_TEXTURE_H
#define __F3D_TEXTURE_H

#include <memory>
#include <cstdint>
#include <iostream>

#include "core/platform.h"
#include "core/types.h"

namespace f3d {
	namespace tree {
		class F3D_API Texture {
		public:
			Texture(uint32_t width, uint32_t height, uint32_t mip_levels, eTextureTypeBits type, eTextureAddressMode address_mode[3]);
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

			eTextureTypeBits			_type;
			eTextureAddressMode			_address_mode[3];
		};
	}// tree::
}// f3d::

#endif