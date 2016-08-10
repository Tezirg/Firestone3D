#pragma once

#ifndef _F3D_VULKAN_H
#define _F3D_VULKAN_H

#include <cstdlib>
#include <iostream>
#include <vulkan/vulkan.h>
#include "utils/logging.h"

namespace f3d {
	namespace utils {

		static PFN_vkGetPhysicalDeviceSurfaceSupportKHR			fpGetPhysicalDeviceSurfaceSupportKHR = 0;
		static PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR	fpGetPhysicalDeviceSurfaceCapabilitiesKHR = 0;
		static PFN_vkGetPhysicalDeviceSurfaceFormatsKHR			fpGetPhysicalDeviceSurfaceFormatsKHR = 0;
		static PFN_vkGetPhysicalDeviceSurfacePresentModesKHR	fpGetPhysicalDeviceSurfacePresentModesKHR = 0;
		static PFN_vkCreateSwapchainKHR							fpCreateSwapchainKHR = 0;
		static PFN_vkDestroySwapchainKHR						fpDestroySwapchainKHR = 0;
		static PFN_vkGetSwapchainImagesKHR						fpGetSwapchainImagesKHR = 0;
		static PFN_vkAcquireNextImageKHR						fpAcquireNextImageKHR = 0;
		static PFN_vkQueuePresentKHR							fpQueuePresentKHR = 0;

		bool													queryInstancePFN(VkInstance instance);
		bool													queryDevicePFN(VkDevice device);
		char const*												vkResultToString(VkResult code);

	}
}

#define F3D_ASSERT_VK(value, expected, err_msg)														\
{																									\
	if (value != expected) {																		\
		std::string		msg("F3D vulkan assertion failed [");										\
		msg.append(f3d::utils::vkResultToString(value));											\
		msg.append(" != ");																			\
		msg.append(f3d::utils::vkResultToString(expected));											\
		msg.append("]: ");																			\
		msg.append(err_msg);																		\
		F3D_FATAL_ERROR(msg.c_str());																\
	}																								\
}

// Macro to get a procedure address based on a vulkan instance
#define GET_INSTANCE_PROC_ADDR(inst, entrypoint)													\
{																									\
    f3d::utils::fp##entrypoint = (PFN_vk##entrypoint) vkGetInstanceProcAddr(inst, "vk"#entrypoint); \
}

// Macro to get a procedure address based on a vulkan device
#define GET_DEVICE_PROC_ADDR(dev, entrypoint)											            \
{																						            \
    f3d::utils::fp##entrypoint = (PFN_vk##entrypoint) vkGetDeviceProcAddr(dev, "vk"#entrypoint);    \
}

#endif