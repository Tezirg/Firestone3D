#pragma once

#ifndef _F3D_TEXTURED_PROG_H
#define _F3D_TEXTURED_PROG_H

#include "core/plateform.h"
#include "utils/vulkan.h"
#include "core/programs/program.h"


const static char texture_vert_spv[] = "texture.vert.spv";
const static char texture_frag_spv[] = "texture.frag.spv";

namespace f3d {
	namespace core {
		namespace prog {
			class F3D_API TexturedProgram : public f3d::core::Program {
			public:
				TexturedProgram(VkDevice device);
				~TexturedProgram();

				void						initVkPipeline(VkRenderPass& renderpass, uint32_t subpass);

				virtual void				initVkPipelineInfos();
				/*!
				* @func createVklayout
				* @brief Create the program layout for the unique uniform attribute
				*/
				virtual void				initVkLayout();

				/*!
				* @func createVkDescriptorPool
				* @brief Create a descriptor pool to allocate the uniform buffer
				*/
				virtual void				initVkDecriptorPool();

				/*!
				* @func createVkDescriptorSets
				* @brief Create this program descriptor sets
				*/
				virtual void				initVkDescriptorSets();
			public:
				VkDescriptorSet					world_set;
				VkDescriptorSet					model_set;
				VkDescriptorSet					sampler_set;

			private:
				VkVertexInputAttributeDescription		_vi_attr[3]; //!< pipeline has 3 inputs
				VkVertexInputBindingDescription			_vi_bind[3]; //!< pipeline has one binding per attribute (total 3)
			};
		}
	}
}

#endif