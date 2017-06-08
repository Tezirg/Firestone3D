#pragma once 

#ifndef _F3D_RENDER_PASS_H
#define _F3D_RENDER_PASS_H

#include "core/platform.h"
#include "core/types.h"
#include "core/device.h"
#include "utils/vulkan.h"
#include "core/window.h"
#include "core/physical_device.h"
#include "tree/scene.h"
#include "core/programs/program.h"

namespace f3d {
	namespace core {

		class F3D_API RenderPass {
		public:
			RenderPass(f3d::RenderPassType type, 
						std::shared_ptr<f3d::core::Device>& device, 
						std::shared_ptr<f3d::core::PhysicalDevice>& physical, 
						std::shared_ptr<f3d::core::Window>& window);			
			virtual ~RenderPass();
			
			RenderPass(const RenderPass& cpy_oth) = delete; // No copy
			RenderPass(const RenderPass&& move_oth) = delete; //No move
			RenderPass& operator=(const RenderPass& cpy_oth) = delete; //No assignement
			RenderPass&	operator=(const RenderPass&& move_oth) = delete; // No move assignement
			
			RenderPassType			getType() const;
			void					setType(const RenderPassType val);

			virtual void			render(VkCommandBuffer cmd, std::shared_ptr<f3d::tree::Scene> scene) = 0;
		protected:
			f3d::core::Program*		getProgram(const F3D_Mask mask);
			void					setProgram(f3d::core::Program *program);
		private:
			f3d::RenderPassType									_type;
		protected:
			std::map< F3D_Mask, std::shared_ptr<f3d::core::Program> >			_programs;
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