#include "tree/texture_impl.h"

namespace f3d {
	namespace tree {

		TextureImpl::TextureImpl(uint32_t width, uint32_t height, uint32_t mip_levels, eTextureTypeBits type, eTextureAddressMode mode[3],
			std::shared_ptr<f3d::core::PhysicalDevice>& phys, std::shared_ptr<f3d::core::Device>& device) :
			Texture::Texture(width, height, mip_levels, type, mode),
			_physical(phys), _device(device), 
			DescriptorContainer::DescriptorContainer(phys, device) 
		{ }

		TextureImpl::~TextureImpl() {
			vkDestroySampler(_device->vk_device, vk_sampler, nullptr);
			vkDestroyImageView(_device->vk_device, vk_view, nullptr);
			vkDestroyImage(_device->vk_device, vk_image, nullptr);

			vkFreeMemory(_device->vk_device, vk_memory, nullptr);
			std::cout << "Destructor: " << __FILE__ << std::endl;
		}

		bool								TextureImpl::initializeLinearTiling(uint32_t width, uint32_t height, void *data, uint32_t size, VkFormat format) {
			VkMemoryRequirements			memReqs;

			vk_image_layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			createImage(format);
			vkGetImageMemoryRequirements(_device->vk_device, vk_image, &memReqs);
			initMemory(data, size, memReqs);
			_device->initImageLayout(vk_image, VK_IMAGE_LAYOUT_PREINITIALIZED, vk_image_layout, VK_ACCESS_HOST_WRITE_BIT, VK_ACCESS_SHADER_READ_BIT, VK_IMAGE_ASPECT_COLOR_BIT);
			createSampler();
			createView(format);

			//Create descriptor set layout
			DescriptorContainer::addDescriptor(2); //layout (set = 2, binding = 0) uniform sampler2D samplerColor;
			DescriptorContainer::addDescriptorBinding(2, 0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT);

			updateDescriptorSet();

			return true;
		}
		
		void						TextureImpl::createImage(VkFormat format) {
			VkImageCreateInfo		info;
			VkResult				r;

			std::memset(&info, 0, sizeof(VkImageCreateInfo));
			info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
			info.imageType = VK_IMAGE_TYPE_2D;
			info.format = format;
			info.mipLevels = _mip_levels;
			info.arrayLayers = 1;
			info.samples = VK_SAMPLE_COUNT_1_BIT;
			info.tiling = VK_IMAGE_TILING_LINEAR;
			info.usage = VK_IMAGE_USAGE_SAMPLED_BIT;
			info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
			info.initialLayout = VK_IMAGE_LAYOUT_PREINITIALIZED;
			info.extent = { _width, _height, 1 };
			r = vkCreateImage(_device->vk_device, &info, NULL, &vk_image);
			F3D_ASSERT_VK(r, VK_SUCCESS, "Create texture image");
		}

		void						TextureImpl::initMemory(void *data, uint32_t size, VkMemoryRequirements& memReqs) {
			VkMemoryAllocateInfo	info;
			VkResult				r;
			void					*pData;
			const uint32_t			step = 1024 * 1024 * 4;
			const uint32_t			total = size;

			std::memset(&info, 0, sizeof(VkMemoryAllocateInfo));
			info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
			info.allocationSize = memReqs.size;
			info.memoryTypeIndex = _physical->getMemoryIndex(memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
			r = vkAllocateMemory(_device->vk_device, &info, NULL, &vk_memory);
			F3D_ASSERT_VK(r, VK_SUCCESS, "Allocating texture memory");
			r = vkBindImageMemory(_device->vk_device, vk_image, vk_memory, 0);
			F3D_ASSERT_VK(r, VK_SUCCESS, "Binding texture memory");
			

			 /*
			r = vkMapMemory(_device->vk_device, vk_memory, 0, total, 0, &pData);
			F3D_ASSERT_VK(r, VK_SUCCESS, "Mapping texture memory");
			std::memcpy(pData, (char *)data, total);
			vkUnmapMemory(_device->vk_device, vk_memory);
			// */

			 // /*
			for (uint32_t i = 0; i < total; i += step) {
				VkDeviceSize mapsize = total > (i + step) ? step : (total - i);
				r = vkMapMemory(_device->vk_device, vk_memory, i, mapsize, 0, &pData);
				F3D_ASSERT_VK(r, VK_SUCCESS, "Mapping texture memory");
				std::memcpy(pData, (char *)data + i, mapsize);
				vkUnmapMemory(_device->vk_device, vk_memory);
			}
			// */
		}
		void							TextureImpl::createSampler() {
			VkResult					r;
			VkSamplerCreateInfo			info;

			std::memset(&info, 0, sizeof(VkSamplerCreateInfo));
			info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
			info.magFilter = VK_FILTER_LINEAR;
			info.minFilter = VK_FILTER_LINEAR;
			info.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
			F3D_TEXTURE_2_VULKAN(_address_mode[0], info.addressModeU);
			F3D_TEXTURE_2_VULKAN(_address_mode[1], info.addressModeV);
			F3D_TEXTURE_2_VULKAN(_address_mode[2], info.addressModeW);
			info.mipLodBias = 0.0f;
			info.compareEnable = VK_FALSE;
			info.compareOp = VK_COMPARE_OP_NEVER;
			info.minLod = 0.0f;
			info.maxLod = 0.0f;
			if (_physical->vk_physical_device_features.samplerAnisotropy == VK_TRUE) { // Enable anisotropic filtering
				info.maxAnisotropy = _physical->vk_physical_device_props.limits.maxSamplerAnisotropy; // Use max. level of anisotropy by default
				info.anisotropyEnable = VK_TRUE;
			}
			else { // The device does not support anisotropic filtering		
				info.maxAnisotropy = 1.0;
				info.anisotropyEnable = VK_FALSE;
			}
			info.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
			r = vkCreateSampler(_device->vk_device, &info, nullptr, &vk_sampler);
			F3D_ASSERT_VK(r, VK_SUCCESS, "Createing texture sampelr");
		}

		void							TextureImpl::createView(VkFormat format) {
			VkResult					r;
			VkImageViewCreateInfo		info;

			std::memset(&info, 0, sizeof(VkImageViewCreateInfo));
			info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			info.viewType = VK_IMAGE_VIEW_TYPE_2D;
			info.image = vk_image;
			info.format = format;
			info.components.r = VK_COMPONENT_SWIZZLE_R;
			info.components.g = VK_COMPONENT_SWIZZLE_G;
			info.components.b = VK_COMPONENT_SWIZZLE_B;
			info.components.a = VK_COMPONENT_SWIZZLE_A;
			info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			info.subresourceRange.baseMipLevel = 0;
			info.subresourceRange.baseArrayLayer = 0;
			info.subresourceRange.layerCount = 1;
			info.subresourceRange.levelCount = 1;
			r = vkCreateImageView(_device->vk_device, &info, NULL, &vk_view);
			F3D_ASSERT_VK(r, VK_SUCCESS, "Createing texture ImageView");
		}


		VkDescriptorSet					TextureImpl::getDescriptorSet() {
			return DescriptorContainer::getDescriptorSet(2);
		}

		void							TextureImpl::updateDescriptorSet() {
			VkWriteDescriptorSet		pWrites;
			VkDescriptorImageInfo		sampler_info;

			std::memset(&pWrites, 0, sizeof(VkWriteDescriptorSet));
			pWrites.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			pWrites.dstSet = DescriptorContainer::getDescriptorSet(2);
			pWrites.descriptorCount = 1;
			pWrites.descriptorType = VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			pWrites.pImageInfo = &sampler_info;
			sampler_info.imageView = vk_view;
			sampler_info.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			sampler_info.sampler = vk_sampler;
			vkUpdateDescriptorSets(_device->vk_device, 1, &pWrites, 0, 0);
		}
	}// tree::
}// f3d::