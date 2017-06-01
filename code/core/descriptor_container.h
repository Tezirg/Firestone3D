#ifndef _F3D_DESCRIPTOR_CONTAINER_H
#define _F3D_DESCRIPTOR_CONTAINER_H

#include <iostream>
#include <map>
#include <vector>
#include <utility>

#include "core/platform.h"
#include "core/physical_device.h"
#include "core/device.h"

namespace f3d {
	namespace core {
		class DescriptorContainer {
		public:
			DescriptorContainer(std::shared_ptr< f3d::core::PhysicalDevice >& phys, std::shared_ptr< f3d::core::Device >& device);
			~DescriptorContainer();

			bool	addDescriptor(uint32_t setIndex);
			bool	removeDescriptor(uint32_t setIndex);

			bool	addDescriptorBinding(uint32_t setIndex, uint32_t bindingIndex, VkDescriptorType type, VkShaderStageFlags stage);
			bool	removeDescriptorBinding(uint32_t setIndex, uint32_t bindingIndex);

			VkDescriptorSetLayout	getDescriptorLayout(uint32_t setIndex);
			VkDescriptorSet			getDescriptorSet(uint32_t setIndex);
		private:
			void					updateContainer();
			void					resetContainer();
			void					cleanContainer();
		private:
			std::shared_ptr<f3d::core::PhysicalDevice>				_physical;
			std::shared_ptr<f3d::core::Device>						_device;

			VkDescriptorPool										_pool;
			bool													_dirty_pool;

			std::map<uint32_t, VkDescriptorSet>						_sets;
			std::map<uint32_t, VkDescriptorSetLayout>				_sets_layout;
			std::multimap<uint32_t, VkDescriptorSetLayoutBinding>	_bindings;
		};
	} //core::
} //f3d::

#endif