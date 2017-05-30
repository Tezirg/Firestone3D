#pragma once

#ifndef _F3D_P_0001_0000_0005_0003_H
#define _F3D_P_0001_0000_0005_0003_H

#include "core/platform.h"
#include "core/types.h"
#include "tree/mesh_impl.h"
#include "tree/scene_impl.h"
#include "utils/vulkan.h"
#include "core/programs/program.h"

namespace f3d {
	namespace core {
		namespace prog {
			class F3D_API Program_0001_0000_0005_0003 : public f3d::core::Program {
			public:
				Program_0001_0000_0005_0003(VkDevice device);
				~Program_0001_0000_0005_0003();

				void						initVkPipeline(VkRenderPass& renderpass, uint32_t subpass);

				virtual void				initVkPipelineInfos();
				/*!
				* @func createVklayout
				* @brief Create the program layout for the unique uniform attribute
				*/
				virtual void				initVkLayout();

				bool						drawToCommandBuffer(VkCommandBuffer& cmd, f3d::tree::Mesh& mesh, f3d::tree::Scene& scene);
			private:
				VkVertexInputAttributeDescription		_vi_attr[3]; //!< pipeline has 3 inputs
				VkVertexInputBindingDescription			_vi_bind[3]; //!< pipeline has one binding per attribute (total 3)
			};
		}
	}
}

#endif