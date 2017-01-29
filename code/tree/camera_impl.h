#pragma once

#ifndef __F3D_CAMERA_IMPL_H
#define __F3D_CAMERA_IMPL_H

#include <memory>
#include <cstdint>
#include <assimp/camera.h>

#include "f3d.h"
#include "utils/vulkan.h"
#include "core/physical_device.h"
#include "core/device.h"

namespace f3d {
	namespace tree {
		
		class CameraImpl final : public Camera {
		public:
			CameraImpl(std::shared_ptr< f3d::core::PhysicalDevice >& phys, std::shared_ptr< f3d::core::Device > device);
			~CameraImpl();

			void				updateAttribute();
		private:
			void				createAttribute();
		public:
			VkBuffer						_buffer;
			VkDeviceMemory					_memory;
		private:
			std::shared_ptr<f3d::core::PhysicalDevice>	_physical;
			std::shared_ptr<f3d::core::Device>			_device;
			std::shared_ptr<aiCamera>					_ai_camera;
		};
	}
}

#endif