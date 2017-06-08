#pragma once

#ifndef _F3D_RENDERER_IMPL_H
#define _F3D_RENDERER_IMPL_H

#include <map>
#include <memory>
#include <utility>

#include "core/platform.h"
#include "core/renderer.h"
#include "core/settings.h"
#include "utils/vulkan.h"
#include "core/device.h"
#include "core/physical_device.h"
#include "tree/scene_impl.h"
#include "core/window_impl.h"
#include "renderpasses/render_pass.h"
#include "renderpasses/rp_simple.h"

namespace f3d {
	namespace core {
		class F3D_API RendererImpl: public Renderer {
		public:
			RendererImpl(std::shared_ptr<f3d::core::Settings> & settings,	
						 std::shared_ptr<f3d::core::Device>& device,
						 std::shared_ptr<f3d::core::PhysicalDevice>& physical_device,
						 std::shared_ptr<f3d::core::Window>& window);
			~RendererImpl();

			RendererImpl(RendererImpl& copy_oth) = delete; //No copies
			RendererImpl(RendererImpl&& move_oth) = delete; //No moves
			RendererImpl& operator=(RendererImpl& copy_oth) = delete; //No copy assignement
			RendererImpl& operator=(RendererImpl&& move_oth) = delete; //No move assignement

			void	reset();
			void	render(std::shared_ptr<f3d::tree::Scene> scene);
			void	display();
			void	computeCommandBuffers(std::shared_ptr<f3d::tree::Scene> scene);

		private:
			void			initCommandBuffers();

		public:
			VkCommandBuffer													*vk_commands;
			uint32_t														vk_command_count;
			VkCommandPool													vk_command_pool;
			bool															valid_commands;
		private:
			std::shared_ptr<f3d::core::Device>								_device;
			std::shared_ptr<f3d::core::PhysicalDevice>						_physical;
			std::shared_ptr<f3d::core::Window>								_window;
			std::map< f3d::RenderPassType, std::unique_ptr<RenderPass> >	_renders;
		};
	}
}


#endif