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

			VkBuffer				getVertexBuffer();
			VkBuffer				getNormalBuffer();
			VkBuffer				getIndexBuffer();
			VkBuffer				getUvBuffer();

			VkDescriptorSet			getUniform();
			bool					updateUniform(glm::mat4& model);
		private:
			bool					createAttribute(VkDeviceMemory& mem, uint32_t mem_size, VkBufferUsageFlags usage, VkBuffer& buffer);
			bool					updateAttribute(void *data, VkDeviceMemory& mem, uint64_t size);
			bool					deleteAttribute(VkDeviceMemory& mem, VkBuffer& buffer);
		private:
			std::shared_ptr<f3d::core::PhysicalDevice>	_phys;
			std::shared_ptr<f3d::core::Device>			_device;


			VkDescriptorPool		_uniform_pool;
			VkDescriptorSetLayout	_uniform_layout;
			VkDescriptorSet			_uniform_set;
			VkBuffer				_uniform_buf;
			VkDeviceMemory			_uniform_mem;


			VkBuffer				_vertex_buf;
			VkDeviceMemory			_vertex_mem;

			VkBuffer				_normal_buf;
			VkDeviceMemory			_normal_mem;
			
			VkBuffer				_index_buf;
			VkDeviceMemory			_index_mem;

			VkBuffer				_uv_buf;
			VkDeviceMemory			_uv_mem;
		};
	}
}

#endif