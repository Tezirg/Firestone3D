#include "mesh_impl.h"

namespace f3d {
	namespace tree{
		MeshImpl::MeshImpl(std::shared_ptr<f3d::core::PhysicalDevice>& phys, 
							std::shared_ptr<f3d::core::Device>& device) :
			_phys(phys), _device(device) {
		}


		MeshImpl::~MeshImpl() {
		}
		
		bool			MeshImpl::makeRenderReady() {
			createAttribute(_vertex_mem, _vertices.size() * sizeof(float), _vertex_buf);
			updateAttribute(_vertices.data(), _vertex_mem, _vertices.size() * sizeof(float));
			_vertices.clear();

			createAttribute(_normal_mem, _normals.size() * sizeof(float), _normal_buf);
			updateAttribute(_normals.data(), _normal_mem, _normals.size() * sizeof(float));
			_normals.clear();

			createAttribute(_index_mem, _indices.size() * sizeof(uint32_t), _index_buf);
			updateAttribute(_indices.data(), _index_mem, _indices.size() * sizeof(uint32_t));
			_indices.clear();

			_ready = true;
			return true;
		}

		bool						MeshImpl::createAttribute(VkDeviceMemory& mem, uint32_t mem_size, VkBuffer& buffer) {
			VkResult				r;
			VkBufferCreateInfo		buff_info;
			VkMemoryRequirements	mem_reqs;
			VkMemoryAllocateInfo	mem_info;

			std::memset(&buff_info, 0, sizeof(VkBufferCreateInfo));
			buff_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			buff_info.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
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
			F3D_ASSERT_VK(r, VK_SUCCESS, "Can't map buffer memory");

			std::memcpy(pData, data, size);
			vkUnmapMemory(_device->vk_device, mem);
			return true;
		}
	}
}