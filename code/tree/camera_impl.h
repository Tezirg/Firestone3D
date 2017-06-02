#pragma once

#ifndef __F3D_CAMERA_IMPL_H
#define __F3D_CAMERA_IMPL_H

#include <memory>
#include <cstdint>
#include <assimp/camera.h>

#include "core/platform.h"
#include "utils/vulkan.h"
#include "core/physical_device.h"
#include "core/device.h"
#include "core/descriptor_container.h"
#include "core/attribute_container.h"
#include "tree/camera.h"

namespace f3d {
	namespace tree {
		class F3D_API CameraImpl final : 
			public Camera,
			protected f3d::core::DescriptorContainer,
			protected f3d::core::AttributeContainer {
		public:
			CameraImpl(std::shared_ptr< f3d::core::PhysicalDevice >& phys, std::shared_ptr< f3d::core::Device >& device);
			~CameraImpl();

			void				writeAttribute();
			VkDescriptorSet		getDescriptorSet();
			void				writeDescriptorSet();
		private:
			std::shared_ptr<f3d::core::PhysicalDevice>	_physical;
			std::shared_ptr<f3d::core::Device>			_device;
		};
	} // tree::
} // f3d::

#endif