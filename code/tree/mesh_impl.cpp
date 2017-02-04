#include "mesh_impl.h"

namespace f3d {
	namespace tree{
		MeshImpl::MeshImpl(std::shared_ptr<f3d::core::PhysicalDevice>& phys, 
							std::shared_ptr<f3d::core::Device>& device) :
			_phys(phys), _device(device) {

			createAttribute(_uniform_mem, sizeof(float) * 16, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, _uniform_buf);


			VkResult							r;
			VkDescriptorPoolCreateInfo			poolinfo;
			VkDescriptorPoolSize				poolsize;
			VkDescriptorSetLayoutBinding		uniforminfo;
			VkDescriptorSetLayoutCreateInfo		layoutinfo;
			VkDescriptorSetAllocateInfo			setinfo;

			std::memset(&poolsize, 0, sizeof(VkDescriptorPoolSize));
			poolsize.descriptorCount = 1;
			poolsize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;


			std::memset(&poolinfo, 0, sizeof(VkDescriptorPoolCreateInfo));
			poolinfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
			poolinfo.maxSets = 1;
			poolinfo.poolSizeCount = 1;
			poolinfo.pPoolSizes = &poolsize;
			r = vkCreateDescriptorPool(device->vk_device, &poolinfo, NULL, &_uniform_pool);
			F3D_ASSERT_VK(r, VK_SUCCESS, "Mesh descriptor pool allocate fail");


			std::memset(&uniforminfo, 0, sizeof(VkDescriptorSetLayoutBinding));
			uniforminfo.binding = 0;
			uniforminfo.descriptorCount = 1;
			uniforminfo.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			uniforminfo.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
			std::memset(&layoutinfo, 0, sizeof(VkDescriptorSetLayoutCreateInfo));
			layoutinfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
			layoutinfo.bindingCount = 1;
			layoutinfo.pBindings = &uniforminfo;
			r = vkCreateDescriptorSetLayout(_device->vk_device, &layoutinfo, NULL, &_uniform_layout);
			F3D_ASSERT_VK(r, VK_SUCCESS, "Create emsh desc set layout failed");

			std::memset(&setinfo, 0, sizeof(VkDescriptorSetAllocateInfo));
			setinfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
			setinfo.descriptorPool = _uniform_pool;
			setinfo.descriptorSetCount = 1;
			setinfo.pSetLayouts = &_uniform_layout;
			r = vkAllocateDescriptorSets(_device->vk_device, &setinfo, &_uniform_set);
			F3D_ASSERT_VK(r, VK_SUCCESS, "Mesh uniform descriptor set allocate fail");

			VkDescriptorBufferInfo	bufinfo;
			bufinfo.buffer = _uniform_buf;
			bufinfo.offset = 0;
			bufinfo.range = VK_WHOLE_SIZE;

			VkWriteDescriptorSet	desc_write;
			std::memset(&desc_write, 0, sizeof(VkWriteDescriptorSet));
			desc_write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			desc_write.descriptorCount = 1;
			desc_write.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			desc_write.dstSet = _uniform_set;
			desc_write.dstBinding = 0;
			desc_write.pBufferInfo = &bufinfo;
			vkUpdateDescriptorSets(_device->vk_device, 1, &desc_write, 0, NULL);

		}


		MeshImpl::~MeshImpl() {
		}
		
		VkDescriptorSet			MeshImpl::getUniform() {
			return _uniform_set;
		}

		bool					MeshImpl::updateUniform(glm::mat4& model) {
			return updateAttribute(glm::value_ptr(model), _uniform_mem, sizeof(float) * 16);
		}

		VkBuffer				MeshImpl::getVertexBuffer() { return _vertex_buf;  }
		VkBuffer				MeshImpl::getNormalBuffer() { return _normal_buf;  }
		VkBuffer				MeshImpl::getIndexBuffer() { return _index_buf; }

		bool			MeshImpl::makeRenderReady() {
			createAttribute(_vertex_mem, _vertices.size() * sizeof(float), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, _vertex_buf);
			updateAttribute(_vertices.data(), _vertex_mem, _vertices.size() * sizeof(float));
			_vertices.clear();

			createAttribute(_normal_mem, _normals.size() * sizeof(float), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, _normal_buf);
			updateAttribute(_normals.data(), _normal_mem, _normals.size() * sizeof(float));
			_normals.clear();

			createAttribute(_index_mem, _indices.size() * sizeof(uint32_t), VK_BUFFER_USAGE_INDEX_BUFFER_BIT, _index_buf);
			updateAttribute(_indices.data(), _index_mem, _indices.size() * sizeof(uint32_t));
			_indices.clear();

			_ready = true;
			return true;
		}

		bool						MeshImpl::createAttribute(VkDeviceMemory& mem, uint32_t mem_size, VkBufferUsageFlags usage, VkBuffer& buffer) {
			VkResult				r;
			VkBufferCreateInfo		buff_info;
			VkMemoryRequirements	mem_reqs;
			VkMemoryAllocateInfo	mem_info;

			std::memset(&buff_info, 0, sizeof(VkBufferCreateInfo));
			buff_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			buff_info.usage = usage;
			buff_info.size = mem_size;
			r = vkCreateBuffer(_device->vk_device, &buff_info, NULL, &buffer);
			F3D_ASSERT_VK(r, VK_SUCCESS, "Creation vk buffer failed");

			vkGetBufferMemoryRequirements(_device->vk_device, buffer, &mem_reqs);
			std::memset(&mem_info, 0, sizeof(VkMemoryAllocateInfo));
			mem_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
			mem_info.memoryTypeIndex = _phys->getMemoryIndex(mem_reqs.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
			mem_info.allocationSize = mem_reqs.size;
			r = vkAllocateMemory(_device->vk_device, &mem_info, NULL, &mem);
			F3D_ASSERT_VK(r, VK_SUCCESS, "Buffer mem allocation failed");

			r = vkBindBufferMemory(_device->vk_device, buffer, mem, 0);
			F3D_ASSERT_VK(r, VK_SUCCESS, "Bind memory to buffer fail");
			return true;

		}

		bool					MeshImpl::deleteAttribute(VkDeviceMemory& mem, VkBuffer& buffer) {
			vkDestroyBuffer(_device->vk_device, buffer, NULL);
			vkFreeMemory(_device->vk_device, mem, NULL);
			return true;
		}

		bool					MeshImpl::updateAttribute(void *data, VkDeviceMemory& mem, uint64_t size) {
			VkResult			r;
			char				*pData;

			r = vkMapMemory(_device->vk_device, mem, 0, size, 0, (void **)&pData);
			F3D_ASSERT_VK(r, VK_SUCCESS, "Can't map mesh buffer memory");

			std::memcpy(pData, data, size);
			vkUnmapMemory(_device->vk_device, mem);
			return true;
		}
	}
}