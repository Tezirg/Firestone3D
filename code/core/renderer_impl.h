#pragma once

#ifndef _F3D_RENDERER_IMPL_H
#define _F3D_RENDERER_IMPL_H

#include <map>

#include "f3d.h"
#include "utils/vulkan.h"
#include "core/device.h"
#include "tree/scene_impl.h"
#include "core/window_impl.h"
#include "renderpasses/render_pass.h"
#include "renderpasses/rp_simple.h"

namespace f3d {
	namespace core {
		class RendererImpl: public Renderer {
		public:
			RendererImpl(std::shared_ptr<f3d::core::Settings> & settings,	
						 std::shared_ptr<f3d::core::Device>& device,
						 std::shared_ptr<f3d::core::Window>& window);
			~RendererImpl();

			void	render(std::shared_ptr<f3d::tree::Scene> scene);
			void	display();
		public:
			VkRect2D		vk_scissor;
			VkViewport		vk_viewport;
		private:

		private:
			std::shared_ptr<f3d::core::Device>							_device;
			std::shared_ptr<f3d::core::Window>							_window;
			std::map< eRenderPassType, std::unique_ptr<RenderPass> >	_renders;
		};
	}
}


#endif