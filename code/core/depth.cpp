#include "depth.h"

namespace f3d {
	namespace core {
		Depth::Depth(std::shared_ptr<f3d::core::Device>& device,
					 std::shared_ptr<f3d::core::PhysicalDevice>& physical,
					 uint32_t width, uint32_t height) 
			: _width(width), _height(height), _phys(physical), _device(device) {
			vk_format = VK_FORMAT_D16_UNORM;
			createVk();
		}

		Depth::~Depth() {
			vkDestroyImageView(_device->vk_device, vk_view, NULL);
			vkFreeMemory(_device->vk_device, vk_memory, NULL);
			vkDestroyImage(_device->vk_device, vk_image, NULL);

			std::cout << "Destructor: " << __FILE__ << std::endl;
		}

		void						Depth::createVk() {
			VkResult				r;
			bool					r_bool;
			VkMemoryRequirements	mem_reqs;
			VkImageCreateInfo		image_info;
			VkImageViewCreateInfo	view_info;
			VkMemoryAllocateInfo	mem_info;


			std::memset(&image_info, 0, sizeof(VkImageCreateInfo));
			image_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
			image_info.pNext = NULL;
			image_info.imageType = VK_IMAGE_TYPE_2D;
			image_info.format = vk_format;
			image_info.extent.width = _width;
			image_info.extent.height = _height;
			image_info.extent.depth = 1;
			image_info.mipLevels = 1;
			image_info.arrayLayers = 1;
			image_info.samples = VK_SAMPLE_COUNT_1_BIT;
			image_info.tiling = VK_IMAGE_TILING_OPTIMAL;
			image_info.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
			image_info.flags = 0;

			std::memset(&view_info, 0, sizeof(VkImageViewCreateInfo));
			view_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			view_info.pNext = NULL;
			view_info.image = VK_NULL_HANDLE;
			view_info.format = vk_format;
			view_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
			view_info.subresourceRange.baseMipLevel = 0;
			view_info.subresourceRange.levelCount = 1;
			view_info.subresourceRange.baseArrayLayer = 0;
			view_info.subresourceRange.layerCount = 1;
			view_info.flags = 0;
			view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;

			r = vkCreateImage(_device->vk_device, &image_info, NULL, &vk_image);
			F3D_ASSERT_VK(r, VK_SUCCESS, "Cannot create depth image");

			vkGetImageMemoryRequirements(_device->vk_device, vk_image, &mem_reqs);

			mem_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
			mem_info.pNext = NULL;
			mem_info.allocationSize = mem_reqs.size;
			mem_info.memoryTypeIndex = _phys->getMemoryIndex(mem_reqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
			F3D_ASSERT(mem_info.memoryTypeIndex != -1, "Cant find depth memory type");

			r = vkAllocateMemory(_device->vk_device, &mem_info, NULL, &vk_memory);
			F3D_ASSERT_VK(r, VK_SUCCESS, "Allocating depth buffer memory failed");

			r = vkBindImageMemory(_device->vk_device, vk_image, vk_memory, 0);
			F3D_ASSERT_VK(r, VK_SUCCESS, "Cant vind depth image to depth buffer");

			view_info.image = vk_image;
			r = vkCreateImageView(_device->vk_device, &view_info, NULL, &vk_view);
			F3D_ASSERT_VK(r, VK_SUCCESS, "Create depth image view failed");

			r_bool = _device->initImageLayout(vk_image, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL, 
											 0, VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT, VK_IMAGE_ASPECT_DEPTH_BIT);
			F3D_ASSERT(r_bool == true, "Init depth buffer image layout failed");
		}
	}
}