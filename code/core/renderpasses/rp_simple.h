#pragma once

#ifndef _F3D_SIMPLE_RENDERPASS_H
#define _F3D_SIMPLE_RENDERPASS_H

#include <memory>

#include "f3d.h"
#include "render_pass.h"
#include "core/device.h"
#include "core/programs/prog_flat.h"
#include "utils/vulkan.h"

namespace f3d {
	namespace core {
		namespace renderpass {
			class SimpleRenderPass : public RenderPass {
			public:
				SimpleRenderPass(std::shared_ptr<f3d::core::Device>& device, VkFormat color);
				~SimpleRenderPass();

				void		render(VkCommandBuffer cmd, VkFramebuffer frame, std::shared_ptr<f3d::tree::Scene> scene);
			private:
				void		initRenderPass();
				void		updateDescriptorSet(f3d::tree::Camera& cam);
			private:
				std::shared_ptr<f3d::core::Device>					_device;
				VkFormat											_color_format;
				std::unique_ptr<f3d::core::prog::FlatProgram>		_prog;
				VkDescriptorSet										_set;
				VkClearValue										_clear[2]; //!< Color & depth clear values
				VkAttachmentDescription								_attachments[2]; //!< Color & Depth description
				VkAttachmentReference								_color_reference; //!< Color attachement details
				VkAttachmentReference								_depth_reference;//!< Depth attachement details
			};
		}
	}
}

#endif