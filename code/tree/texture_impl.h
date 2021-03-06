#pragma once

#ifndef _F3D_TEXTURE_IMPL_H
#define _F3D_TEXTURE_IMPL_H

#include <vulkan/vulkan.h>

#include "core/plateform.h"
#include "core/device.h"
#include "tree/texture.h"

namespace f3d {
	namespace tree {
		class F3D_API TextureImpl : public f3d::tree::Texture {
			public:
				TextureImpl(uint32_t width, uint32_t height, uint32_t mip_levels, eTextureType type, eTextureAddressMode mode,
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
				VkDescriptorImageInfo	vk_descriptor_info;

				VkDescriptorSetLayout	vk_desc_layout;
				VkDescriptorPool		vk_desc_pool;
				VkDescriptorSet			vk_descriptor;
		};
	}// tree::
}// f3d::

#endif
