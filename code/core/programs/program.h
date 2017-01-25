#pragma once

#ifndef _F3D_PROGRAM_H
#define _F3D_PROGRAM_H

#include <iostream>
#include <fstream>
#include <map>

#include "f3d.h"
#include "utils/vulkan.h"

namespace f3d {
	namespace core {

		class Program {
		public:
			Program(VkDevice device);
			virtual ~Program();

			void				bind(VkCommandBuffer& cmd);
		protected:
			bool				createSpvShader(const std::string& filename, VkShaderModule *shader);

			virtual void		initVkLayout() = 0;
			virtual void		initVkPipelineInfos();
			virtual void		initVkDecriptorPool() = 0;

		public:
			VkDevice				vk_device; //Device to create from
			VkPipeline				vk_pipeline; //!< Vulkan pipeline native type
			VkPipelineLayout		vk_pipeline_layout;
			VkPipelineCache			vk_pipeline_cache;
			VkDescriptorSetLayout	*vk_desc_layout;
			VkDescriptorPool		vk_desc_pool;
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