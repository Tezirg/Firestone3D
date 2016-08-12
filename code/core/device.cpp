#include "device.h"

namespace f3d {
	namespace core {
		Device::Device(VkInstance instance, VkPhysicalDevice physical)
			: _vk_instance(instance), _vk_physical_device(physical)
		{

			//Get nb family
			vkGetPhysicalDeviceQueueFamilyProperties(_vk_physical_device, &_queue_families_count, NULL);
			F3D_ASSERT(_queue_families_count != 0, "Physical device doesn't have any queue");
			//Get queues fammilies infos
			vk_queue_family_properties = new VkQueueFamilyProperties[_queue_families_count];
			vkGetPhysicalDeviceQueueFamilyProperties(_vk_physical_device, &_queue_families_count, vk_queue_family_properties);

			vk_device = createDevice();
			F3D_ASSERT(f3d::utils::queryDevicePFN(vk_device), "Failed to retreive Vulkan device functions");
		}

		Device::~Device() {

		}

		uint32_t							Device::getQueueFamilyIndex(bool present, VkQueueFlags flags) {
			uint32_t						queue_family = UINT32_MAX;
			uint32_t						flagQueueNodeIndex;
			uint32_t						presentQueueNodeIndex;


			// Iterate over each queue to learn whether it supports presenting:
			VkBool32 *supportsPresent = new VkBool32[_queue_families_count];
			for (uint32_t i = 0; i < _queue_families_count; i++) {
				supportsPresent[i] = glfwGetPhysicalDevicePresentationSupport(_vk_instance, _vk_physical_device, i);
//				f3d::utils::fpGetPhysicalDeviceSurfaceSupportKHR(_vk_physical_device, i, _vk_surface, &supportsPresent[i]);
			}

			flagQueueNodeIndex = _queue_families_count;
			presentQueueNodeIndex = _queue_families_count + 1;
			for (uint32_t i = 0; i < _queue_families_count; i++) {
				if (supportsPresent[i] == VK_TRUE) // Current node support present
					presentQueueNodeIndex = i;

				if ((vk_queue_family_properties[i].queueFlags & flags) == flags) {//Current node support flags
					flagQueueNodeIndex = i;
					if (present && flagQueueNodeIndex == presentQueueNodeIndex) { //Current also support present as asked, we have a match
						queue_family = presentQueueNodeIndex;
						break;
					}
					else { //No presentation to take care of, we have a match		
						queue_family = flagQueueNodeIndex;
						break;
					}
				}
			}

			delete [] supportsPresent;
			F3D_ASSERT(queue_family != UINT32_MAX, "Can't find a queue matching family properties")
			return queue_family;
		}

		uint32_t						Device::getQueueFamilyCount(uint32_t family_index) {
			if (family_index >= _queue_families_count) {
				return 0;
			}
			return vk_queue_family_properties[family_index].queueCount;
		}

		VkQueue							Device::getQueue(uint32_t family_index, uint32_t index) {
			if (vk_queues[family_index][index] == VK_NULL_HANDLE) {
				vkGetDeviceQueue(vk_device, family_index, index, &(vk_queues[family_index][index]));
			}
			return vk_queues[family_index][index];
		}

		VkCommandPool					Device::getCommandPool(uint32_t family_index) {
			VkResult					r;
			VkCommandPoolCreateInfo		pool_infos;

			if (vk_pools[family_index] == VK_NULL_HANDLE) {
				std::memset(&pool_infos, 0, sizeof(VkCommandPoolCreateInfo));
				pool_infos.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
				pool_infos.queueFamilyIndex = family_index;
				r = vkCreateCommandPool(vk_device, &pool_infos, NULL, &(vk_pools[family_index]));
				F3D_ASSERT_VK(r, VK_SUCCESS, "Cannot create command pool for this device");
			}
			return vk_pools[family_index];
		}

		VkDevice						Device::createDevice() {
			VkResult					r;
			VkDevice					device;
			VkDeviceCreateInfo			device_infos;
			VkDeviceQueueCreateInfo		*queue_infos;
			float						**queue_priorities;
			VkPhysicalDeviceFeatures	features;
			VkDeviceQueueCreateInfo		using_queue;
			bool						found_queue;

			std::memset(&features, 0, sizeof(VkPhysicalDeviceFeatures));
			features.shaderClipDistance = VK_TRUE;
			features.shaderCullDistance = VK_TRUE;


			queue_infos = new VkDeviceQueueCreateInfo[_queue_families_count];
			queue_priorities = new float *[_queue_families_count];
			for (uint32_t i = 0; i < _queue_families_count; i++) {
				uint32_t fam_count = getQueueFamilyCount(i);
				queue_priorities[i] = new float[fam_count];
				for (uint32_t j = 0; j < fam_count; j++)
					queue_priorities[i][j] = 1.0;

				std::memset(&(queue_infos[i]), 0, sizeof(VkDeviceQueueCreateInfo));
				queue_infos[i].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
				queue_infos[i].queueFamilyIndex = i;// _queue_family;
				queue_infos[i].queueCount = fam_count;
				queue_infos[i].pQueuePriorities = queue_priorities[i];

				//Allocate queue container
				vk_queues.insert(std::pair<uint32_t, VkQueue *>(i, new VkQueue[fam_count]));
				std::memset(vk_queues[i], 0, fam_count * sizeof(VkQueue));

				//Allocate command pool container
				vk_pools.insert(std::pair<uint32_t, VkCommandPool>(i, VK_NULL_HANDLE));
			}

			found_queue = false;
			for (uint32_t i = 0; i < _queue_families_count; i++) {
				if ((vk_queue_family_properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) == VK_QUEUE_GRAPHICS_BIT) {
					std::memset(&using_queue, 0, sizeof(VkDeviceQueueCreateInfo));
					using_queue.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
					using_queue.queueCount = 1;
					using_queue.pQueuePriorities = new float(1.0);
					using_queue.queueFamilyIndex = i;
					found_queue = true;
					break;
				}
			}

			F3D_ASSERT(found_queue, "Cannot find a queue with VK_QUEUE_GRAPHICS_BIT to compute graphics");

			std::memset(&device_infos, 0, sizeof(VkDeviceCreateInfo));
			device_infos.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
			device_infos.enabledExtensionCount = 0;
			device_infos.ppEnabledExtensionNames = 0;
			device_infos.queueCreateInfoCount = 1;//_queue_families_count;
			device_infos.pQueueCreateInfos = &using_queue;
			device_infos.pEnabledFeatures = &features;

			r = vkCreateDevice(_vk_physical_device, &device_infos, NULL, &device);
			F3D_ASSERT_VK(r, VK_SUCCESS, "Creating vulkan device failed")
			return device;
		}

		bool			Device::waitIdle() {
			return vkDeviceWaitIdle(vk_device) == VK_SUCCESS;
		}

		bool								Device::initImageLayout(VkImage image, VkImageLayout old_image_layout, VkImageLayout new_image_layout,
			VkAccessFlags srcAccessMask, VkAccessFlags destAccessMask, VkImageAspectFlags aspectMask) {
			VkResult						r;
			VkCommandBuffer					cmd;
			VkCommandBufferAllocateInfo		cmd_info;
			VkCommandBufferInheritanceInfo	cmd_buf_hinfo;
			VkCommandBufferBeginInfo		cmd_buf_info;
			VkFence							nullFence = VK_NULL_HANDLE;
			VkSubmitInfo					submit_info;
			uint32_t						queue_family;

			queue_family = getQueueFamilyIndex(false, VK_QUEUE_GRAPHICS_BIT);
			cmd_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
			cmd_info.pNext = NULL;
			cmd_info.commandPool = getCommandPool(queue_family);
			cmd_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
			cmd_info.commandBufferCount = 1;

			r = vkAllocateCommandBuffers(vk_device, &cmd_info, &cmd);
			F3D_ASSERT_VK(r, VK_SUCCESS, "Allocating Image layout command buffer fails");

			cmd_buf_hinfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO;
			cmd_buf_hinfo.pNext = NULL;
			cmd_buf_hinfo.renderPass = VK_NULL_HANDLE;
			cmd_buf_hinfo.subpass = 0;
			cmd_buf_hinfo.framebuffer = VK_NULL_HANDLE;
			cmd_buf_hinfo.occlusionQueryEnable = VK_FALSE;
			cmd_buf_hinfo.queryFlags = 0;
			cmd_buf_hinfo.pipelineStatistics = 0;

			cmd_buf_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			cmd_buf_info.pNext = NULL;
			cmd_buf_info.flags = 0;
			cmd_buf_info.pInheritanceInfo = &cmd_buf_hinfo;
			vkBeginCommandBuffer(cmd, &cmd_buf_info);

			setImageLayout(cmd, image, old_image_layout, new_image_layout, srcAccessMask, destAccessMask, aspectMask);
			vkEndCommandBuffer(cmd);

			submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
			submit_info.pNext = NULL;
			submit_info.waitSemaphoreCount = 0;
			submit_info.pWaitSemaphores = NULL;
			submit_info.pWaitDstStageMask = NULL;
			submit_info.commandBufferCount = 1;
			submit_info.pCommandBuffers = &cmd;
			submit_info.signalSemaphoreCount = 0;
			submit_info.pSignalSemaphores = NULL;
			r = vkQueueSubmit(getQueue(queue_family, 0), 1, &submit_info, nullFence);
			F3D_ASSERT_VK(r, VK_SUCCESS, "Submit queue for image layout init fails");

			r = vkQueueWaitIdle(getQueue(queue_family, 0));
			F3D_ASSERT_VK(r, VK_SUCCESS, "Queue wait for image layout init fails");

			vkFreeCommandBuffers(vk_device, getCommandPool(queue_family), 1, &cmd);
			return true;
		}

		bool						Device::setImageLayout(VkCommandBuffer cmd, VkImage image, VkImageLayout old_image_layout, VkImageLayout new_image_layout,
			VkAccessFlags srcAccessMask, VkAccessFlags destAccessMask, VkImageAspectFlags aspectMask) {
			VkImageMemoryBarrier	image_memory_barrier;
			VkPipelineStageFlags	src_stages = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			VkPipelineStageFlags	dest_stages = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;

			std::memset(&image_memory_barrier, 0, sizeof(VkImageMemoryBarrier));
			image_memory_barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
			image_memory_barrier.pNext = NULL;
			image_memory_barrier.srcAccessMask = srcAccessMask;
			image_memory_barrier.dstAccessMask = destAccessMask;
			image_memory_barrier.oldLayout = old_image_layout;
			image_memory_barrier.newLayout = new_image_layout;
			image_memory_barrier.image = image;
			image_memory_barrier.subresourceRange = { aspectMask, 0, 1, 0, 1 };

			vkCmdPipelineBarrier(cmd, src_stages, dest_stages, 0, 0, NULL, 0, NULL, 1, &image_memory_barrier);
			return true;
		}
	}
}