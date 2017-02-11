#pragma once

#ifndef _F3D_TEXTURED_PROG_H
#define _F3D_TEXTURED_PROG_H

#include "f3d.h"
#include "utils/vulkan.h"
#include "core/programs/program.h"


const static char flat_vert_spv[] = "texture.vert.spv";
const static char flat_frag_spv[] = "texture.frag.spv";

namespace f3d {
	namespace core {
		namespace prog {
			class TexturedProgram : public f3d::core::Program {
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
			private:
				VkVertexInputAttributeDescription		_vi_attr[2]; //!< pipeline has 2 inputs
				VkVertexInputBindingDescription			_vi_bind[2]; //!< pipeline has one binding per input (total 2)
			};
		}
	}
}

#endif