#pragma once

#ifndef _F3D_TEXTURE_IMPL_H
#define _F3D_TEXTURE_IMPL_H

#include <vulkan/vulkan.h>

#include "core/platform.h"
#include "core/physical_device.h"
#include "core/device.h"
#include "core/descriptor_container.h"
#include "tree/texture.h"
#include "utils/f3d_to_vk.h"

namespace f3d {
	namespace tree {
		class F3D_API TextureImpl : 
			public f3d::tree::Texture,
			protected f3d::core::DescriptorContainer {
		public:
			TextureImpl(uint32_t width, uint32_t height, uint32_t mip_levels, eTextureTypeBits type, eTextureAddressMode mode[3],
						std::shared_ptr<f3d::core::PhysicalDevice>& phys, std::shared_ptr<f3d::core::Device>& device);
			~TextureImpl();
			bool				initializeLinearTiling(uint32_t width, uint32_t height, void *data, uint32_t size, VkFormat format);
			VkDescriptorSet		getDescriptorSet();
			void				updateDescriptorSet();

		private:
			void			createImage(VkFormat format);
			void			initMemory(void *data, uint32_t size, VkMemoryRequirements& memReqs);
			void			createSampler(void);
			void			createView(VkFormat format);
		private:
			std::shared_ptr<f3d::core::PhysicalDevice>	_physical;
			std::shared_ptr<f3d::core::Device>			_device;
		public:
			VkSampler				vk_sampler;
			VkImage					vk_image;
			VkImageLayout			vk_image_layout;
			VkDeviceMemory			vk_memory;
			VkImageView				vk_view;
		};
	}// tree::
}// f3d::

#endif
