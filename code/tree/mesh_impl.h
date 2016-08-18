#pragma once

#ifndef __F3D_MESH_IMPL_H
#define __F3D_MESH_IMPL_H

#include <memory>
#include <cstdint>
#include <assimp/scene.h>
#include <assimp/mesh.h>

#include "f3d.h"
#include "utils/vulkan.h"
#include "core/physical_device.h"
#include "core/device.h"

namespace f3d {
	namespace tree {
		class MeshImpl : public Mesh {
		public:
			MeshImpl(std::shared_ptr<f3d::core::PhysicalDevice>& phys, std::shared_ptr<f3d::core::Device>& device);
			~MeshImpl();
	
			virtual bool			makeRenderReady();
		private:
			bool					createAttribute(VkDeviceMemory& mem, uint32_t mem_size, VkBuffer& buffer);
			bool					updateAttribute(void *data, VkDeviceMemory& mem, uint64_t size);
			bool					deleteAttribute(VkDeviceMemory& mem, VkBuffer& buffer);
		private:
			std::shared_ptr<f3d::core::PhysicalDevice>	_phys;
			std::shared_ptr<f3d::core::Device>			_device;

			VkBuffer				_vertex_buf;
			VkDeviceMemory			_vertex_mem;
			VkBuffer				_normal_buf;
			VkDeviceMemory			_normal_mem;
			VkBuffer				_index_buf;
			VkDeviceMemory			_index_mem;
		};
	}
}

#endif