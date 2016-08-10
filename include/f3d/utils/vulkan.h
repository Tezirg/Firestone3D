#pragma once


#ifndef _F3D_VULKAN_H
#define _F3D_VULKAN_H

#include <cstdlib>
#include <iostream>
#include <vulkan/vulkan.h>
#include "utils/logging.h"

// Macro to get a procedure address based on a vulkan instance
#define GET_INSTANCE_PROC_ADDR(inst, entrypoint)													\
{																									\
    f3d::utils::fp##entrypoint = (PFN_vk##entrypoint) vkGetInstanceProcAddr(inst, "vk"#entrypoint); \
    if (fp##entrypoint == NULL)																		\
	{																								\
		ERR_EXIT("Can't find vk"#entrypoint)														\
    }																								\
}

// Macro to get a procedure address based on a vulkan device
#define GET_DEVICE_PROC_ADDR(dev, entrypoint)											            \
{																						            \
    f3d::utils::fp##entrypoint = (PFN_vk##entrypoint) vkGetDeviceProcAddr(dev, "vk"#entrypoint);    \
    if (fp##entrypoint == NULL)																	    \
	{																								\
		std::cerr << "Can't find vk"#entrypoint << std::endl;										\
        exit(EXIT_FAILURE);																			\
    }																								\
}

namespace f3d {
	namespace utils {

		static PFN_vkGetPhysicalDeviceSurfaceSupportKHR			fpGetPhysicalDeviceSurfaceSupportKHR;
		static PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR	fpGetPhysicalDeviceSurfaceCapabilitiesKHR;
		static PFN_vkGetPhysicalDeviceSurfaceFormatsKHR			fpGetPhysicalDeviceSurfaceFormatsKHR;
		static PFN_vkGetPhysicalDeviceSurfacePresentModesKHR	fpGetPhysicalDeviceSurfacePresentModesKHR;
		static PFN_vkCreateSwapchainKHR							fpCreateSwapchainKHR;
		static PFN_vkDestroySwapchainKHR						fpDestroySwapchainKHR;
		static PFN_vkGetSwapchainImagesKHR						fpGetSwapchainImagesKHR;
		static PFN_vkAcquireNextImageKHR						fpAcquireNextImageKHR;
		static PFN_vkQueuePresentKHR							fpQueuePresentKHR;

		bool													queryInstancePFN(VkInstance instance);
		bool													queryDevicePFN(VkDevice device);
		char const*												vkResultToString(VkResult code);

	}
}

#define F3D_ASSERT_VK(value, expected, err_msg)														\
{																									\
	if (value != expected) {																		\
		std::string		msg("F3D assert failed [");													\
		msg.append(f3d::utils::vkResultToString(value));											\
		msg.append(" != ");																			\
		msg.append(f3d::utils::vkResultToString(expected));											\
		msg.append("] : ");																			\
		msg.append(err_msg);																		\
		F3D_FATAL_ERROR(err_msg)																	\
	}																								\
}

#endif