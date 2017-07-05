#pragma once

#ifndef _F3D_P_0002_0001_0000_0000_H
#define _F3D_P_0002_0001_0000_0000_H

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
			class F3D_API Program_0002_0001_0000_0000 :
				public f3d::core::Program,
				protected f3d::core::DescriptorContainer {
			public:
				Program_0002_0001_0000_0000(std::shared_ptr< f3d::core::PhysicalDevice >& phys, std::shared_ptr< f3d::core::Device >& device);
				~Program_0002_0001_0000_0000();

				virtual void							initVkPipelineInfos();
				virtual void							initVkLayout();

				void									initVkPipeline(VkRenderPass& renderpass, uint32_t subpass);
				bool									drawToCommandBuffer(VkCommandBuffer& cmd, f3d::tree::Mesh& mesh, f3d::tree::Scene& scene);
			private:
				VkVertexInputAttributeDescription		_vi_attr[2];
				VkVertexInputBindingDescription			_vi_bind[2];
			};
		}
	}
}

#endif