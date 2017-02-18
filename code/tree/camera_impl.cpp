#include "camera_impl.h"

namespace f3d {
	namespace tree {
		CameraImpl::CameraImpl(std::shared_ptr< f3d::core::PhysicalDevice >& phys, std::shared_ptr< f3d::core::Device > device)
			: _physical(phys), _device(device), _ai_camera(new aiCamera)
		{
			applyPreset(F3D_CAMERA_PRESET_DEFAULT);
			setName(std::string("DefaultCamera"));
			setPerspective(_fov, _aspect, 0.1f, 1000.0f);
			lookAt(glm::vec3(0.0f, 0.0f, 250.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			createAttribute();
			updateAttribute();

			VkResult							r;
			VkDescriptorSetLayoutBinding		layout_bindings;
			VkDescriptorSetLayoutCreateInfo		desc_layout_info;
			VkDescriptorPoolSize				pool_types;
			VkDescriptorPoolCreateInfo			pool_info;
			VkDescriptorSetAllocateInfo			desc_set_alloc;

			//Create camera uniform descriptor set layout
			//layout (set = 0, binding = 0) uniform camera;
			std::memset(&layout_bindings, 0, sizeof(VkDescriptorSetLayoutBinding));
			layout_bindings.binding = 0;
			layout_bindings.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			layout_bindings.descriptorCount = 1;
			layout_bindings.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
			layout_bindings.pImmutableSamplers = NULL;
			std::memset(&desc_layout_info, 0, sizeof(VkDescriptorSetLayoutCreateInfo));
			desc_layout_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
			desc_layout_info.bindingCount = 1;
			desc_layout_info.pBindings = &layout_bindings;
			r = vkCreateDescriptorSetLayout(_device->vk_device, &desc_layout_info, NULL, &_desc_layout);
			F3D_ASSERT_VK(r, VK_SUCCESS, "Camera Create descriptor set layout failed");


			//Create camera Uniform descriptor pool
			pool_types.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			pool_types.descriptorCount = 1;
			std::memset(&pool_info, 0, sizeof(VkDescriptorPoolCreateInfo));
			pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
			pool_info.poolSizeCount = 1;
			pool_info.maxSets = 1;
			pool_info.pPoolSizes = &pool_types;
			r = vkCreateDescriptorPool(_device->vk_device, &pool_info, NULL, &_desc_pool);
			F3D_ASSERT_VK(r, VK_SUCCESS, "Camera Descriptor pool creation failed");

			//Allocate descriptor set from pool 
			std::memset(&desc_set_alloc, 0, sizeof(VkDescriptorSetAllocateInfo));
			desc_set_alloc.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
			desc_set_alloc.descriptorPool = _desc_pool;
			desc_set_alloc.descriptorSetCount = 1;
			desc_set_alloc.pSetLayouts = &_desc_layout;
			r = vkAllocateDescriptorSets(device->vk_device, &desc_set_alloc, &_descriptor);
			F3D_ASSERT_VK(r, VK_SUCCESS, "Camera Descriptor set allocation failed");

			updateDescriptorSet();
		}

		CameraImpl::~CameraImpl() {
			vkFreeDescriptorSets(_device->vk_device, _desc_pool, 1, &_descriptor);
			vkDestroyDescriptorPool(_device->vk_device, _desc_pool, nullptr);
			vkDestroyDescriptorSetLayout(_device->vk_device, _desc_layout, nullptr);
			vkFreeMemory(_device->vk_device, _memory, nullptr);
			vkDestroyBuffer(_device->vk_device, _buffer, nullptr);
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

			std::memcpy(pData, glm::value_ptr(_VP), 16 * sizeof(float));
			vkUnmapMemory(_device->vk_device, _memory);
		}

		VkDescriptorSet							CameraImpl::getDescriptorSet() {
			return _descriptor;
		}

		void									CameraImpl::updateDescriptorSet() {
			VkWriteDescriptorSet				pWrites;
			VkDescriptorBufferInfo				buffer_info;

			//Bind camera buffer with this camera descriptor set
			std::memset(&pWrites, 0, sizeof(VkWriteDescriptorSet));
			pWrites.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			pWrites.dstSet = _descriptor;
			pWrites.descriptorCount = 1;
			pWrites.descriptorType = VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			pWrites.pBufferInfo = &buffer_info;
			buffer_info.offset = 0;
			buffer_info.range = 16 * sizeof(float);
			buffer_info.buffer = _buffer;
			vkUpdateDescriptorSets(_device->vk_device, 1, &pWrites, 0, nullptr);
		}

	}
}