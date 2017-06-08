#include "core/attribute_container.h"

namespace f3d {
	namespace core {
		AttributeContainer::AttributeContainer(std::shared_ptr< f3d::core::PhysicalDevice >& phys, std::shared_ptr< f3d::core::Device >& dev) :
			_physical(phys), _device(dev) {
		}
		AttributeContainer::~AttributeContainer() {
			for (auto attr = _memory.begin(); attr != _memory.end(); ++attr)
				removeAttribute(attr->first);

			std::cout << "Destructor: " << __FILE__ << std::endl;
		}


		bool						AttributeContainer::addAttribute(uint32_t attrIndex, uint32_t size, VkBufferUsageFlags usage) {
			VkResult				r;
			VkBufferCreateInfo		buffer_info;
			VkMemoryRequirements	mem_req;
			VkMemoryAllocateInfo	mem_info;

			//Create the buffer
			std::memset(&buffer_info, 0, sizeof(VkBufferCreateInfo));
			buffer_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			buffer_info.size = size;
			buffer_info.usage = usage;
			r = vkCreateBuffer(_device->vk_device, &buffer_info, NULL, &_buffer[attrIndex]);
			F3D_ASSERT_VK(r, VK_SUCCESS, "Attribute buffer creation failed");

			//Allocate corresponding memory
			vkGetBufferMemoryRequirements(_device->vk_device, _buffer[attrIndex], &mem_req);
			std::memset(&mem_info, 0, sizeof(VkMemoryAllocateInfo));
			mem_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
			mem_info.memoryTypeIndex = _physical->getMemoryIndex(mem_req.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
			mem_info.allocationSize = mem_req.size;
			r = vkAllocateMemory(_device->vk_device, &mem_info, NULL, &_memory[attrIndex]);
			F3D_ASSERT_VK(r, VK_SUCCESS, "Attribute memory allocation failed");

			//Link buffer and memory together
			r = vkBindBufferMemory(_device->vk_device, _buffer[attrIndex], _memory[attrIndex], 0);
			F3D_ASSERT_VK(r, VK_SUCCESS, "Bind memory to buffer fail");

			//Save this attribute size
			_size[attrIndex] = size;

			return true;
		}

		bool				AttributeContainer::removeAttribute(uint32_t attrIndex) {
			auto			b = _buffer.find(attrIndex);
			auto			m = _memory.find(attrIndex);
			auto			s = _size.find(attrIndex);

			if (b == _buffer.end() || m == _memory.end() || s == _size.end())
				return false;
			vkDestroyBuffer(_device->vk_device, b->second, nullptr);
			vkFreeMemory(_device->vk_device, m->second, nullptr);
			_buffer.erase(b);
			_memory.erase(m);
			_size.erase(s);
			return true;
		}

		bool					AttributeContainer::updateAttribute(uint32_t attrIndex, void *data, uint32_t offset, uint32_t size) {
			VkResult			r;
			void *				pData;
			auto				m = _memory.find(attrIndex);
			const uint32_t		step = 1024 * 1024 * 4;

			if (m == _memory.end())
				return false;

			if (size <= step) {
				r = vkMapMemory(_device->vk_device, m->second, offset, size, 0, (void **)&pData);
				F3D_ASSERT_VK(r, VK_SUCCESS, "Can't map attribute memory");
				std::memcpy(pData, data, size);
				vkUnmapMemory(_device->vk_device, m->second);
			}
			else {
				for (uint32_t i = 0; i < size; i += step) {
					VkDeviceSize mapsize = size > (i + step) ? step : (size - i);
					r = vkMapMemory(_device->vk_device, m->second, offset + i, mapsize, 0, &pData);
					F3D_ASSERT_VK(r, VK_SUCCESS, "Can't map attribute memory");
					std::memcpy(pData, (char *)data + i, mapsize);
					vkUnmapMemory(_device->vk_device, m->second);
				}
			}

			return true;
		}

		VkBuffer				AttributeContainer::getAttributeBuffer(uint32_t attrIndex) {
			auto it = _buffer.find(attrIndex);
			if (it == _buffer.end())
				return VK_NULL_HANDLE;
			return it->second;
		}

		VkDeviceMemory			AttributeContainer::getAttributeMemory(uint32_t attrIndex) {
			auto it = _memory.find(attrIndex);
			if (it == _memory.end())
				return VK_NULL_HANDLE;
			return it->second;
		}

		uint32_t		AttributeContainer::getAttributeSize(uint32_t attrIndex) const {
			auto it = _size.find(attrIndex);
			if (it == _size.end())
				return 0;
			return it->second;
		}

		uint32_t		AttributeContainer::getAttributeSizeStd140(uint32_t attrIndex) const {
			auto it = _size.find(attrIndex);
			if (it == _size.end())
				return 0;
			uint32_t padded = it->second;
			padded += it->second % (sizeof(float) * 4); // std140 alignement on vec4 size
			return padded;
		}

	}// core::
} //f3d::