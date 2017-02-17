#pragma once

#ifndef _F3D_DEVICE_H
#define _F3D_DEVICE_H

#include <map>
#include <iostream>
#include <cstdint>
#include <vulkan/vulkan.h>
#include <glfw/glfw3.h>

#include "core/plateform.h"
#include "utils/vulkan.h"
#include "core/physical_device.h"

namespace f3d {
	namespace core {
		/*!
		* @class Device
		* @brief A device is associated with a physical device (GPU) and a Vulkan context
		* The universe is allocated and submited throught the vulkan device native type
		*/
		class F3D_API Device
		{
		public:
			/*!
			* @func Device
			* @brief Constructor
			* @param[in] innstance Vulkan instance in use
			* @param[in] physical GPU in use
			*/
			Device(VkInstance instance, VkPhysicalDevice physical);
			~Device();

			/*!
			* @func waitIdle
			* @brief Waits for the device to complete all operations
			*/
			bool			waitIdle();

			/*!
			* @func getQueueFamilyIndex
			* @brief Getter on queue family
			* @param[in] present Does the family support presentation ?
			* @param[in] flags Vulkan flags to support (VK_QUEUE_GRAPHICS_BIT for example)
			* @return the family index
			*/
			uint32_t		getQueueFamilyIndex(bool present, VkQueueFlags flags, VkSurfaceKHR surface = 0);

			/*!
			* @func getQueueFamilyCount
			* @brief Getter on the size of a queue family
			* @param[in] family_index The index of the family to count
			* @return The size of a family
			*/
			uint32_t		getQueueFamilyCount(uint32_t family_index);

			/*!
			* @func getQueue
			* @param[in] family_index The family of this queue
			* @param[in] index The index within its family
			* @return A vulkan native queue type
			*/
			VkQueue			getQueue(uint32_t family_index, uint32_t index);

			/*!
			* @func getCommandPool
			* @brief Getter on a queue familly command pool
			* @param[in] The family index
			* @return A command pool for this family
			*/
			VkCommandPool	getCommandPool(uint32_t familly_index);

			/*!
			* @func initImageLayout
			* @brief Changes a image layout by issuing a command to the first graphics queue available
			* @param[in] image The target
			* @param[in] old_image_layout Vulkan params
			* @return true on success
			*/
			bool			initImageLayout(VkImage image, VkImageLayout old_image_layout, VkImageLayout new_image_layout,
				VkAccessFlags srcAccessMask, VkAccessFlags destAccessMask, VkImageAspectFlags aspectMask);

			/*!
			* @func setImageLayout
			* @brief Register a change in the image layout
			* @param[in] cmd the command buffer used for registering
			* @param[in] image Target to change
			* @return true on success
			*/
			bool			setImageLayout(VkCommandBuffer cmd, VkImage image, VkImageLayout old_image_layout, VkImageLayout new_image_layout,
										   VkAccessFlags srcAccessMask, VkAccessFlags destAccessMask, VkImageAspectFlags aspectMask);


		private:
			/*!
			* @func createDevice
			* @rief creates a device form a GPU and Surface combination
			*/
			VkDevice		createDevice();
		public:
			VkDevice							vk_device;//!< Native vulkan device type
			std::map<uint32_t, VkQueue*>		vk_queues;//!< Association familyIndex, array of queues
			VkQueueFamilyProperties				*vk_queue_family_properties; //!< Queue families decriptions
			std::map<uint32_t, VkCommandPool>	vk_pools; //!< Associtation familyIndex, CommandPool

		private:
			VkPhysicalDevice		_vk_physical_device;
			VkInstance				_vk_instance;
			uint32_t				_queue_families_count; //!< Number of families
		};
	}
}

#endif