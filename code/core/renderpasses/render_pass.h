#pragma once 

#ifndef _F3D_RENDER_PASS_H
#define _F3D_RENDER_PASS_H

#include "f3d.h"
#include "utils/vulkan.h"

namespace f3d {
	namespace core {

		enum eRenderPassType {
			F3D_RENDERPASS_UNDEFINED = 0,
			F3D_RENDERPASS_SIMPLE = 1,
			F3D_RENDERPASS_BEGIN_RANGE = F3D_RENDERPASS_UNDEFINED,
			F3D_RENDERPASS_END_RANGE = F3D_RENDERPASS_SIMPLE,
			F3D_RENDERPASS_RANGE_SIZE = (F3D_RENDERPASS_SIMPLE - F3D_RENDERPASS_UNDEFINED + 1),
			F3D_RENDERPASS_MAX_ENUM = 0x7FFFFFFF
		};

		class RenderPass {
		public:
			RenderPass(eRenderPassType type);
			~RenderPass();

			eRenderPassType		getType() const;
			void				setType(const eRenderPassType val);

			virtual void		render(VkCommandBuffer cmd, VkFramebuffer frame, std::shared_ptr<f3d::tree::Scene> scene) = 0;
		private:
			eRenderPassType		_type;

		public:
			uint32_t				vk_subpasses_count;
			VkSubpassDescription	*vk_subpasses;
			VkRenderPass			vk_renderpass;
		};
	}
}

#endif