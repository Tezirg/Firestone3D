#include "camera_impl.h"

namespace f3d {
	namespace tree {
		CameraImpl::CameraImpl(std::shared_ptr< f3d::core::PhysicalDevice >& phys, std::shared_ptr< f3d::core::Device > device) 
			: _physical(phys), _device(device), _ai_camera(new aiCamera) 
		{
			applyPreset(F3D_CAMERA_PRESET_DEFAULT);
			setName(std::string("DefaultCamera"));
			setPerspective(_fov, _aspect, 0.1f, 1000.0f);
			lookAt(glm::vec3(0.0f, 0.0f, 250.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f,  0.0f));
			createAttribute();
		}

		CameraImpl::~CameraImpl() {
		}

		void						CameraImpl::createAttribute() {
			VkResult				r;
			VkBufferCreateInfo		buff_info;
			VkMemoryRequirements	mem_reqs;
			VkMemoryAllocateInfo	mem_info;

			std::memset(&buff_info, 0, sizeof(VkBufferCreateInfo));
			buff_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			buff_info.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
			buff_info.size = 16 * sizeof(float);
			r = vkCreateBuffer(_device->vk_device, &buff_info, NULL, &_buffer);
			F3D_ASSERT_VK(r, VK_SUCCESS, "Creation vk buffer failed");

			vkGetBufferMemoryRequirements(_device->vk_device, _buffer, &mem_reqs);
			std::memset(&mem_info, 0, sizeof(VkMemoryAllocateInfo));
			mem_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
			mem_info.memoryTypeIndex = _physical->getMemoryIndex(mem_reqs.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
			mem_info.allocationSize = mem_reqs.size;
			r = vkAllocateMemory(_device->vk_device, &mem_info, NULL, &_memory);
			F3D_ASSERT_VK(r, VK_SUCCESS, "Buffer mem allocation failed");

			r = vkBindBufferMemory(_device->vk_device, _buffer, _memory, 0);
			F3D_ASSERT_VK(r, VK_SUCCESS, "Bind memory to buffer fail");
		}

		void				CameraImpl::updateAttribute() {
			VkResult			r;
			char				*pData;

			r = vkMapMemory(_device->vk_device, _memory, 0, 16 * sizeof(float), 0, (void **)&pData);
			F3D_ASSERT_VK(r, VK_SUCCESS, "Can't map buffer memory");

			 /*
			static float angle = 0.0f;
			angle += 0.1f;
			angle = fmod(angle, 360.0f);
			glm::mat4 model;
			model = glm::translate(model, glm::vec3(0.0f, -100.0f, 0.0f));
			model = glm::rotate(model, (float)angle, glm::vec3(0.0, 1.0, 0.0));
			model = glm::scale(model, glm::vec3(1000.0f));
			glm::mat4 mvp = _VP * model;
			// */

			std::memcpy(pData, glm::value_ptr(_VP), 16 * sizeof(float));
			vkUnmapMemory(_device->vk_device, _memory);
		}

	}
}