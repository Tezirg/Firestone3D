#pragma once

#ifndef _F3D_PROGRAM_H
#define _F3D_PROGRAM_H

#include <iostream>
#include <fstream>
#include <map>
#include <vector>

#include "core/platform.h"
#include "core/types.h"
#include "tree/mesh_impl.h"
#include "tree/scene_impl.h"
#include "utils/vulkan.h"

namespace f3d {
	namespace core {

		class F3D_API Program {
		public:
			Program(VkDevice device, ProgramMask mask);
			virtual ~Program();
			Program(const Program& oth) = delete; //No copy construction
			Program(const Program&& mov) = delete; //No move construction
			Program&	operator=(const Program& oth) = delete; //No copy assignement
			Program&	operator=(const Program&& move) = delete; //No move assignement

			F3D_Mask								getMask() const;
			bool									hasFlag(const f3d::eShaderInterfaceTypeBits& flag) const;
			bool									hasFlag(const f3d::eShadingTypeBits& flag) const;

			void									bind(VkCommandBuffer& cmd);
			virtual bool							drawToCommandBuffer(VkCommandBuffer& cmd, f3d::tree::Mesh& mesh, f3d::tree::Scene& scene) = 0;
			virtual void							initVkPipeline(VkRenderPass& renderpass, uint32_t subpass) = 0;

		protected:
			bool									createSpvShader(const std::string& filename, VkShaderModule *shader);

			virtual void							initVkLayout() = 0;
			virtual void							initVkPipelineInfos();
		public:
			VkDevice								vk_device; //Device to create from
			ProgramMask								mask;
			VkPipeline								vk_pipeline; //!< Vulkan pipeline native type
			VkPipelineLayout						vk_pipeline_layout;//!< Piepline object
			VkPipelineCache							vk_pipeline_cache; //!< Binary pipeline object
			VkDescriptorSetLayout					*vk_desc_layout; //!< Old desc layout storage !!!! TODO: remove me !!!!
			std::vector<VkDescriptorSetLayout>		vk_desc_layout_vec; //!< Descriptor sets layouts
		protected:
			VkPipelineVertexInputStateCreateInfo	_vi;
			VkPipelineInputAssemblyStateCreateInfo	_ia;
			VkPipelineRasterizationStateCreateInfo	_rs;
			VkPipelineColorBlendStateCreateInfo		_cb;
			VkPipelineDepthStencilStateCreateInfo	_ds;
			VkPipelineViewportStateCreateInfo		_vp;
			VkPipelineMultisampleStateCreateInfo	_ms;
			VkDynamicState							_dynamicStateEnables[VK_DYNAMIC_STATE_RANGE_SIZE];
			VkPipelineDynamicStateCreateInfo		_dynamicState;
			VkGraphicsPipelineCreateInfo			_create_infos;
		};
	}
}

#endif