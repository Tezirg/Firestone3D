#pragma once

#ifndef _F3D_P_0001_0000_0000_0000_H
#define _F3D_P_0001_0000_0000_0000_H

#include "core/platform.h"
#include "core/types.h"
#include "tree/mesh_impl.h"
#include "tree/scene_impl.h"
#include "utils/vulkan.h"
#include "core/programs/program.h"


namespace f3d {
	namespace core {
		namespace prog {
			class F3D_API Program_0001_0000_0000_0000 : public f3d::core::Program {
			public:
				Program_0001_0000_0000_0000(VkDevice device);
				~Program_0001_0000_0000_0000();

				virtual void							initVkPipelineInfos();
				virtual void							initVkLayout();

				void									initVkPipeline(VkRenderPass& renderpass, uint32_t subpass);
				bool									drawToCommandBuffer(VkCommandBuffer& cmd, f3d::tree::Mesh& mesh, f3d::tree::Scene& scene);
			private:
				VkVertexInputAttributeDescription		_vi_attr[2]; //!< pipeline has 2 inputs
				VkVertexInputBindingDescription			_vi_bind[2]; //!< pipeline has one binding per input (total 2)
			};
		}
	}
}

#endif