#pragma once

#ifndef __F3D_TYPES_H
#define __F3D_TYPES_H

#include <cstdint>

namespace f3d {

	typedef uint32_t		F3D_Flags;
	typedef uint64_t		F3D_Mask;

	typedef enum			eColorType {
		F3D_COLOR_UNDEFINED = 0x0,
		F3D_COLOR_AMBIENT = 0x1,
		F3D_COLOR_DIFFUSE = 0x2,
		F3D_COLOR_SPECULAR = 0x4,
		F3D_COLOR_EMISSIVE = 0x8,
		F3D_COLOR_REFLECTIVE = 0x10,
		F3D_COLOR_BEGIN_RANGE = F3D_COLOR_UNDEFINED,
		F3D_COLOR_END_RANGE = F3D_COLOR_REFLECTIVE,
		F3D_COLOR_RANG_SIZE = (F3D_COLOR_END_RANGE - F3D_COLOR_BEGIN_RANGE + 1),
		F3D_COLOR_MAX_ENUM = 0x7FFFFFFF
	}						eColorTypeBits;
	typedef F3D_Flags		ColorTypeFlags;

	typedef enum			eTextureType {
		F3D_TEXTURE_UNDEFINED = 0x0,
		F3D_TEXTURE_DIFFUSE = 0x1,
		F3D_TEXTURE_SPECULAR = 0x2,
		F3D_TEXTURE_AMBIENT = 0x4,
		F3D_TEXTURE_EMISSIVE = 0x8,
		F3D_TEXTURE_HEIGHT = 0x10,
		F3D_TEXTURE_NORMALS = 0x20,
		F3D_TEXTURE_SHININESS = 0x40,
		F3D_TEXTURE_OPACITY = 0x80,
		F3D_TEXTURE_DISPLACEMENT = 0x100,
		F3D_TEXTURE_LIGHTMAP = 0x200,
		F3D_TEXTURE_REFLECTION = 0x400,
		F3D_TEXTURE_BEGIN_RANGE = F3D_TEXTURE_UNDEFINED,
		F3D_TEXTURE_END_RANGE = F3D_TEXTURE_REFLECTION,
		F3D_TEXTURE_RANGE_SIZE = (F3D_TEXTURE_END_RANGE - F3D_TEXTURE_BEGIN_RANGE + 1),
		F3D_TEXTURE_MAX_ENUM = 0x7FFFFFFF
	}						eTextureTypeBits;
	typedef F3D_Flags		TextureTypeFlags;

	typedef enum			eTextureAddressMode {
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
	}						eTextureAddressMode;

	typedef enum			eLightType {
		F3D_LIGHT_UNDEFINED = 0x0,
		F3D_LIGHT_DIRECTIONAL = 0x1,
		F3D_LIGHT_POINT = 0x2,
		F3D_LIGHT_SPOT = 0x4,
		F3D_LIGHT_BEGIN_RANGE = F3D_LIGHT_UNDEFINED,
		F3D_LIGHT_END_RANGE = F3D_LIGHT_SPOT,
		F3D_LIGHT_RANGE_SIZE = (F3D_LIGHT_SPOT - F3D_LIGHT_UNDEFINED + 1),
		F3D_LIGHT_MAX_ENUM = 0x7FFFFFFF
	}						eLightTypeBits;
	typedef F3D_Flags		LightTypeFlags;

	typedef enum			eShadingType {
		F3D_SHADING_UNDEFINED = 0x0,
		F3D_SHADING_FLAT = 0x1,
		F3D_SHADING_GOURAUD = 0x2,
		F3D_SHADING_BLINNPHONG = 0x4,
		F3D_SHADING_TOON = 0x8,
		F3D_SHADING_CUSTOM = 0x10,
		F3D_SHADING_BEGIN_RANGE = F3D_SHADING_UNDEFINED,
		F3D_SHADING_END_RANGE = (F3D_SHADING_CUSTOM - F3D_SHADING_UNDEFINED + 1),
		F3D_SHADING_MAX_ENUM = 0x7FFFFFFF
	}						eShadingTypeBits;
	typedef F3D_Flags		ShadingTypeFlags;

	typedef struct			s_ProgramMaskFields {
		ColorTypeFlags		colors : 16;
		TextureTypeFlags	textures : 16;
		LightTypeFlags		lights : 16;
		ShadingTypeFlags	shading : 16;
	}						ProgramMaskFields;
	typedef union			s_ProgramMask {
		ProgramMaskFields	fields;
		uint64_t			mask;
	}						ProgramMask;

	typedef enum			eRenderPassType {
		F3D_RENDERPASS_UNDEFINED = 0,
		F3D_RENDERPASS_SIMPLE = 1,
		F3D_RENDERPASS_COMPOSED = 2,
		F3D_RENDERPASS_BEGIN_RANGE = F3D_RENDERPASS_UNDEFINED,
		F3D_RENDERPASS_END_RANGE = F3D_RENDERPASS_SIMPLE,
		F3D_RENDERPASS_RANGE_SIZE = (F3D_RENDERPASS_SIMPLE - F3D_RENDERPASS_UNDEFINED + 1),
		F3D_RENDERPASS_MAX_ENUM = 0x7FFFFFFF
	}						RenderPassType;

} // f3d::

#endif