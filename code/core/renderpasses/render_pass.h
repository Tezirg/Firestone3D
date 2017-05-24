#pragma once 

#ifndef _F3D_RENDER_PASS_H
#define _F3D_RENDER_PASS_H

#include "core/platform.h"
#include "core/device.h"
#include "utils/vulkan.h"
#include "core/window.h"
#include "core/physical_device.h"
#include "tree/scene.h"

#include <memory>

namespace f3d {
	namespace core {

		class F3D_API RenderPass {
		public:

			enum eRenderPassType {
				F3D_RENDERPASS_UNDEFINED = 0,
				F3D_RENDERPASS_SIMPLE = 1,
				F3D_RENDERPASS_BEGIN_RANGE = F3D_RENDERPASS_UNDEFINED,
				F3D_RENDERPASS_END_RANGE = F3D_RENDERPASS_SIMPLE,
				F3D_RENDERPASS_RANGE_SIZE = (F3D_RENDERPASS_SIMPLE - F3D_RENDERPASS_UNDEFINED + 1),
				F3D_RENDERPASS_MAX_ENUM = 0x7FFFFFFF
			};

			RenderPass(eRenderPassType type, 
						std::shared_ptr<f3d::core::Device>& device, 
						std::shared_ptr<f3d::core::PhysicalDevice>& physical, 
						std::shared_ptr<f3d::core::Window>& window);			
			virtual ~RenderPass();

			RenderPass(const RenderPass& cpy_oth) = default;
			RenderPass(const RenderPass&& move_oth) = delete;
			RenderPass& operator=(const RenderPass& cpy_oth) = default;
			RenderPass&	operator=(const RenderPass&& move_oth) = delete;


			eRenderPassType		getType() const;
			void				setType(const eRenderPassType val);

			virtual void		render(VkCommandBuffer cmd, std::shared_ptr<f3d::tree::Scene> scene) = 0;

		private:
			eRenderPassType			_type;
		public:
			std::shared_ptr<f3d::core::Device>					device;
			std::shared_ptr<f3d::core::Window>					window;
			std::shared_ptr<f3d::core::PhysicalDevice>			physical;

			VkRect2D				vk_scissor;
			VkViewport				vk_viewport;
			uint32_t				vk_subpasses_count;
			VkSubpassDescription	*vk_subpasses;
			VkRenderPass			vk_renderpass;
			VkImageView	*			vk_views;
			VkFramebuffer *			vk_framebuffers;
		};
	}
}

#endif