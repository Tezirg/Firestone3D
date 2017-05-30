#pragma once

#ifndef __F3D_MATERIAL_IMPL_H
#define __F3D_MATERIAL_IMPL_H

#include <memory>
#include <cstdint>
#include <iostream>
#include <glm/glm.hpp>

#include "core/platform.h"
#include "core/types.h"
#include "core/device.h"
#include "core/physical_device.h"
#include "utils/vulkan.h"
#include "tree/material.h"
#include "tree/texture_impl.h"

namespace f3d {
	namespace tree {
		class F3D_API MaterialImpl : public Material {
		public:
			MaterialImpl(const std::string& name, std::shared_ptr< f3d::core::PhysicalDevice >& phys, std::shared_ptr< f3d::core::Device >& device);
			virtual ~MaterialImpl();

			void				updateAttribute();
			void				createAttribute();
			VkDescriptorSet		getDescriptorSet();
			void				updateDescriptorSet();

		private:
			std::shared_ptr<f3d::core::PhysicalDevice>	_physical;
			std::shared_ptr<f3d::core::Device>			_device;

			VkBuffer									_buffer;
			VkDeviceMemory								_memory;
			VkDescriptorSetLayout						_desc_layout;
			VkDescriptorPool							_desc_pool;
			VkDescriptorSet								_descriptor;

		};
	}// tree::
}// f3d::

#endif