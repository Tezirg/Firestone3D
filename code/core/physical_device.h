#pragma once

#ifndef _F3D_PHYSICAL_DEVICE_H
#define _F3D_PHYSICAL_DEVICE_H

#include <iostream>
#include <cstdint>
#include <vulkan/vulkan.h>

#include "core/plateform.h"
#include "utils/vulkan.h"

namespace f3d {
	namespace core {
		/*!
		* @class PhysicalDevice
		* @brief Wrapper of the vulkan GPU description
		* it describes the used hardware and its memory layout
		*/
		class F3D_API PhysicalDevice
		{
		public:
			/*!
			* @func PhysicalDevice
			* @brief Constructor query hardware and selects the first usable
			* @param[in] context Requires a vulkan context to query hardware capabilities
			*/
			PhysicalDevice(VkInstance instance);
			~PhysicalDevice();

			/*!
			* @func getMemoryIndex
			* @brief Utility function to query memory description on the GPU
			* @return The index in the memory decription array, -1 on failure
			*/
			int		getMemoryIndex(uint32_t typeBits, uint32_t requirementsBits);

			VkPhysicalDevice					vk_physical_device; //!< In use Vulkan native device
			VkPhysicalDeviceProperties			vk_physical_device_props; //!< In use vulkan device properties 
			VkPhysicalDeviceFeatures			vk_physical_device_features; //!< Storage for enabled hardware features
			VkPhysicalDeviceMemoryProperties	vk_physical_device_mem_props;//!< Memory properties of in use hardware 
		private:
			VkInstance							_vk_instance;
			uint32_t							_device_count; //!<Total number of device 
			VkPhysicalDevice					*_devices; //!< Array of _device_count vulkan devices native type
			VkPhysicalDeviceProperties			*_props; //!< Properties for each devices
			VkPhysicalDeviceMemoryProperties	*_mem_props; //! < memory properties for each device
			VkPhysicalDeviceFeatures			*_features; //!< Hardware features for each physical device
		};
	}
}
#endif