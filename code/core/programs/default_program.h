#pragma once

#ifndef _F3D_DEFAULT_PROGRAM_H
#define _F3D_DEFAULT_PROGRAM_H

#include <vector>
#include <sstream>
#include <iomanip>

#include "core/platform.h"
#include "core/types.h"
#include "core/physical_device.h"
#include "core/device.h"
#include "core/programs/program.h"
#include "core/descriptor_container.h"
#include "tree/mesh_impl.h"
#include "tree/scene_impl.h"
#include "utils/vulkan.h"

namespace f3d {
	namespace core {
		namespace prog {
			class F3D_API DefaultProgram :
				public f3d::core::Program,
				protected f3d::core::DescriptorContainer {
			public:
				DefaultProgram(std::shared_ptr< f3d::core::PhysicalDevice >& phys, std::shared_ptr< f3d::core::Device >& device);
				DefaultProgram(std::shared_ptr< f3d::core::PhysicalDevice >& phys, std::shared_ptr< f3d::core::Device >& device, ProgramMask mask);
				~DefaultProgram();

				DefaultProgram(const DefaultProgram& oth) = delete; //No copy construction
				DefaultProgram(const DefaultProgram&& mov) = delete; // No move construction
				DefaultProgram&		operator=(const DefaultProgram& oth) = delete; // No copy assignement
				DefaultProgram&		operator=(const DefaultProgram&& mov) = delete; // No move assignement

				virtual void							initVkPipelineInfos();
				virtual void							initVkLayout();

				void									initVkPipeline(VkRenderPass& renderpass, uint32_t subpass);
				bool									drawToCommandBuffer(VkCommandBuffer& cmd, f3d::tree::Mesh& mesh, f3d::tree::Scene& scene);
			private:
				void									initSetsFromMask();
				void									initAttributesFromMask();
				void									bindDescriptorSets(VkCommandBuffer cmd, VkPipelineBindPoint bindPoint, f3d::tree::Scene& scene, f3d::tree::Mesh& mesh);
				void									bindAttributes(VkCommandBuffer cmd, f3d::tree::Mesh& mesh);
			private:
				std::vector<VkVertexInputAttributeDescription>		_vi_attrs;
				std::vector<VkVertexInputBindingDescription>		_vi_binds;
				std::vector<VkDescriptorSet>						_bound_sets;
				std::vector<VkBuffer>								_bound_buffers;
				std::vector<VkDeviceSize>							_bound_offsets;
			};
		} // prog::
	} // core::
} // f3d::

#endif //_F3D_DEFAULT_PROGRAM_H