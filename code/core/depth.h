#pragma once

#ifndef _F3D_DEPTH_H
#define _F3D_DEPTH_H

#include <iostream>
#include <string>
#include <cstdint>
#include <memory>
#include <vulkan/vulkan.h>

#include "f3d.h"
#include "utils/vulkan.h"

#include "core/physical_device.h"
#include "core/device.h"

namespace f3d {
	namespace core {
		/*!
		 * @class Depth
		 * @brief represents a Z-buffer for use with a framebuffer
		 */
		class Depth
		{
		public:
			/*!
			 * @func Depth
			 * @brief Constructor
			 * @param[in] physical GPU to allocate the depth buffer to
			 * @param[in] device Vulkan device to use with
			 * @param[in] Buffer width in pixels
			 * @param[in] Buffer height in pixels
			 */
			Depth(std::shared_ptr<f3d::core::Device>& device,
				  std::shared_ptr<f3d::core::PhysicalDevice>& physical,
				  uint32_t width,
				  uint32_t height);
			~Depth();
		private:
			void			createVk(); //!< Allocates depth buffer memory
			void			destroyVk(); //!< Fres depth buffer memory
		public:
			VkImage			vk_image; //!< Vulkan iamge type
			VkImageView		vk_view; //!< Vulkan view type from image
			VkDeviceMemory	vk_memory; //!< Allocated memory on GPU
			VkFormat		vk_format; //!< Depth buffer format
		private:
			uint32_t									_width; //!< Depth buffer width
			uint32_t									_height; //!< Depth buffer height
			std::shared_ptr<f3d::core::PhysicalDevice>	_phys;//<! ref to GPU
			std::shared_ptr<f3d::core::Device>			_device; //!< ref to vulkan device
		};
	}
}

#endif