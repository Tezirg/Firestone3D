#pragma once

#ifndef _F3D_SIMPLE_RENDERPASS_H
#define _F3D_SIMPLE_RENDERPASS_H

#include <memory>
#include <iostream>
#include <stack>
#include <list>

#include "core/platform.h"
#include "core/window.h"
#include "core/window_impl.h"
#include "render_pass.h"
#include "core/device.h"
#include "core/depth.h"
#include "core/physical_device.h"
#include "core/programs/prog_flat.h"
#include "core/programs/prog_textured.h"
#include "tree/camera_impl.h"
#include "tree/mesh_impl.h"
#include "tree/texture_impl.h"
#include "utils/vulkan.h"

namespace f3d {
	namespace core {
		namespace renderpass {
			class F3D_API SimpleRenderPass : public RenderPass {
			public:
				SimpleRenderPass(std::shared_ptr<f3d::core::Device>& device, std::shared_ptr<f3d::core::PhysicalDevice>& physical, std::shared_ptr<f3d::core::Window>& window);
				~SimpleRenderPass();

				void												render(VkCommandBuffer cmd, std::shared_ptr<f3d::tree::Scene> scene);
			private:
				void												initRenderPass();
				void												initViews();
				void												initFramebuffers();
				void												cmdDrawObject(VkCommandBuffer cmd, std::shared_ptr< f3d::tree::Scene > scene, f3d::tree::Node* obj);
				void												cmdDrawMesh(VkCommandBuffer cmd, std::shared_ptr< f3d::tree::Scene > scene, f3d::tree::Mesh& mesh);
			private:
				std::unique_ptr<f3d::core::Depth>					_depth; //!< Z-Buffer wrapper
				VkFormat											_color_format; //!< Image format
				std::unique_ptr<f3d::core::prog::FlatProgram>		_flat_prog;
				std::unique_ptr<f3d::core::prog::TexturedProgram>	_texture_prog;
				VkClearValue										_clear[2]; //!< Color & depth clear values
				VkAttachmentDescription								_attachments[2]; //!< Color & Depth description
				VkAttachmentReference								_color_reference; //!< Color attachement details
				VkAttachmentReference								_depth_reference;//!< Depth attachement details

				VkImage												depth_vk_image; //!< Vulkan image type
				VkImageView											depth_vk_view; //!< Vulkan view type from image
				VkDeviceMemory										depth_vk_memory; //!< Allocated memory on GPU
				VkFormat											depth_vk_format; //!< Depth buffer format

				std::stack<glm::mat4>								_matrix; //!< World transformation matrix stack. Used to reproduce glPushMatrix & glPopMatrix

			};
		}
	}
}

#endif