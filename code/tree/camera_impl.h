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
#include "tree/camera.h"

namespace f3d {
	namespace tree {
		
		class F3D_API CameraImpl final : public Camera {
		public:
			CameraImpl(std::shared_ptr< f3d::core::PhysicalDevice >& phys, std::shared_ptr< f3d::core::Device > device);
			~CameraImpl();

			void				updateAttribute();
			void				createAttribute();
			VkDescriptorSet		getDescriptorSet();
			void				updateDescriptorSet();
		public:
			VkBuffer									_buffer;
			VkDeviceMemory								_memory;

		private:
			std::shared_ptr<f3d::core::PhysicalDevice>	_physical;
			std::shared_ptr<f3d::core::Device>			_device;
			std::shared_ptr<aiCamera>					_ai_camera;
			
			VkDescriptorSetLayout						_desc_layout;
			VkDescriptorPool							_desc_pool;
			VkDescriptorSet								_descriptor;
		};
	}
}

#endif