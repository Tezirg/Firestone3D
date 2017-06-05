#pragma once

#ifndef __F3D_LIGHT_IMPL_H
#define __F3D_LIGHT_IMPL_H

#include <memory>
#include <cstdint>
#include <iostream>

#include <assimp/light.h>

#include "core/platform.h"
#include "core/types.h"
#include "core/physical_device.h"
#include "core/device.h"
#include "tree/light.h"

namespace f3d {
	namespace tree {
		class F3D_API LightImpl : public Light {
		public:
			LightImpl(std::shared_ptr< f3d::core::PhysicalDevice >& phys, std::shared_ptr< f3d::core::Device >& device);
			LightImpl(std::shared_ptr< f3d::core::PhysicalDevice >& phys, std::shared_ptr< f3d::core::Device >& device, const f3d::tree::Light& oth);
			LightImpl(std::shared_ptr< f3d::core::PhysicalDevice >& phys, std::shared_ptr< f3d::core::Device >& device, aiLight *light);
			~LightImpl();

			void					updateProperties();
			void					createProperties();
			bool					getProperties(void **ptr, uint32_t & size);

		private:
			std::shared_ptr< f3d::core::PhysicalDevice >	_physical;
			std::shared_ptr< f3d::core::Device >			_device;
			char*											_buffer;
			uint32_t										_buffer_size;

		};
	}
}

#endif