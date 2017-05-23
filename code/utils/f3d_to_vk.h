#pragma once

#ifndef _F3D_2_VK_H
#define _F3D_2_VK_H

#include "tree/texture.h"
#include <vulkan/vulkan.h>

#define F3D_TEXTURE_2_VULKAN(f3d_type, target)														\
{																									\
	switch (f3d_type) {																				\
		case F3D_ADDRESS_REPEAT:																	\
			target = VK_SAMPLER_ADDRESS_MODE_REPEAT;												\
			break;																					\
		case F3D_ADDRESS_CLAMP_EDGE:																\
			target = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;											\
			break;																					\
		case F3D_ADDRESS_CLAMP_BORDER:																\
			target = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;										\
			break;																					\
		case F3D_ADDRESS_MIRROR_REPEAT:																\
			target = VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;										\
			break;																					\
		case F3D_ADDRESS_MIRROR_CLAMP:																\
			target = VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE;									\
			break;																					\
		default:																					\
			target = VK_SAMPLER_ADDRESS_MODE_REPEAT;												\
	}																								\
	std::cout << "Texture mapping is :" << target << std::endl; \
}

#endif