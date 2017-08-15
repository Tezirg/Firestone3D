#pragma once

#ifndef __F3D_MESH_IMPL_H
#define __F3D_MESH_IMPL_H

#include <memory>
#include <cstdint>
#include <assimp/scene.h>
#include <assimp/mesh.h>

#include "core/platform.h"
#include "utils/vulkan.h"
#include "core/physical_device.h"
#include "core/device.h"
#include "core/descriptor_container.h"
#include "core/attribute_container.h"
#include "tree/mesh.h"

namespace f3d {
	namespace tree {
		class F3D_API MeshImpl : 
			public Mesh, 
			protected f3d::core::DescriptorContainer,
			protected f3d::core::AttributeContainer {
		public:
			MeshImpl(std::shared_ptr<f3d::core::PhysicalDevice>& phys, std::shared_ptr<f3d::core::Device>& device);
			~MeshImpl();
	
			virtual bool			makeRenderReady();

			VkBuffer				getVertexBuffer();
			VkBuffer				getNormalBuffer();
			VkBuffer				getIndexBuffer();
			VkBuffer				getUvBuffer();
			VkBuffer				getColorBuffer();
			VkBuffer				getReserved1Buffer();
			VkBuffer				getReserved2Buffer();

			VkDescriptorSet			getDescriptorSet();
			bool					updateDescriptorSet(glm::mat4& model);

		private:
			std::shared_ptr<f3d::core::PhysicalDevice>	_phys;
			std::shared_ptr<f3d::core::Device>			_device;
		};
	}
}

#endif