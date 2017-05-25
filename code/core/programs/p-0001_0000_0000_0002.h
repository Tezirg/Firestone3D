#pragma once

#ifndef _F3D_FLAT_PROG_H
#define _F3D_FLAT_PROG_H

#include "core/platform.h"
#include "core/types.h"
#include "tree/mesh_impl.h"
#include "tree/scene_impl.h"
#include "utils/vulkan.h"
#include "core/programs/program.h"


const static char flat_vert_spv[] = "flat.vert.spv";
const static char flat_frag_spv[] = "flat.frag.spv";

namespace f3d {
	namespace core {
		namespace prog {
			class F3D_API FlatProgram : public f3d::core::Program {
			public:
				FlatProgram(VkDevice device);
				~FlatProgram();

				void									initVkPipeline(VkRenderPass& renderpass, uint32_t subpass);
				virtual void							initVkPipelineInfos();

				/*!
				* @func createVklayout
				* @brief Create the program layout for the unique uniform attribute
				*/
				virtual void							initVkLayout();

				bool									drawToCommandBuffer(VkCommandBuffer& cmd, f3d::tree::Mesh& mesh, f3d::tree::Scene& scene);
			private:
				VkVertexInputAttributeDescription		_vi_attr[2]; //!< pipeline has 2 inputs
				VkVertexInputBindingDescription			_vi_bind[2]; //!< pipeline has one binding per input (total 2)
			};
		}
	}
}

#endif