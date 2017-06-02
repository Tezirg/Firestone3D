#ifndef _F3D_ATTRIBUTE_CONTAINER_H
#define _F3D_ATTRIBUTE_CONTAINER_H

#include <iostream>
#include <map>
#include <vector>

#include "core/platform.h"
#include "core/physical_device.h"
#include "core/device.h"

namespace f3d {
	namespace core {
		class AttributeContainer {
		public:
			AttributeContainer(std::shared_ptr< f3d::core::PhysicalDevice >& phys, std::shared_ptr< f3d::core::Device >& device);
			~AttributeContainer();

			bool			addAttribute(uint32_t attrIndex, uint32_t size, VkBufferUsageFlags usage);
			bool			removeAttribute(uint32_t attrIndex);
			bool			updateAttribute(uint32_t attrIndex, void *data, uint32_t offset, uint32_t size);
			VkBuffer		getAttributeBuffer(uint32_t attrIndex);
			VkDeviceMemory	getAttributeMemory(uint32_t attrIndex);
		private:
			std::shared_ptr<f3d::core::PhysicalDevice>	_physical;
			std::shared_ptr<f3d::core::Device>			_device;
			std::map<uint32_t, VkDeviceMemory>			_memory;
			std::map<uint32_t, VkBuffer>				_buffer;
			std::map<uint32_t, uint32_t>				_size;
		};
	} //core::
} // f3d::

#endif