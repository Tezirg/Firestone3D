#include "render_pass.h"

namespace f3d {
	namespace core {
		RenderPass::RenderPass(eRenderPassType type, 
								std::shared_ptr<f3d::core::Device>& dev, 
								std::shared_ptr<f3d::core::PhysicalDevice>& phys, 
								std::shared_ptr<f3d::core::Window>& win):
			_type(type), device(dev), physical(phys), window(win) {
			std::memset(&vk_viewport, 0, sizeof(vk_viewport));
			vk_viewport.width = (float)window->width();
			vk_viewport.height = (float)window->height();
			vk_viewport.minDepth = 0.0f;
			vk_viewport.maxDepth = 1.0f;

			std::memset(&vk_scissor, 0, sizeof(vk_scissor));
			vk_scissor.extent.width = window->width();
			vk_scissor.extent.height = window->height();
			vk_scissor.offset.x = 0;
			vk_scissor.offset.y = 0;
			vk_views = nullptr;
			vk_framebuffers = nullptr;
		}

		RenderPass::~RenderPass() {
		}

		eRenderPassType		RenderPass::getType() const {
			return _type;
		}

		void				RenderPass::setType(const eRenderPassType type) {
			_type = type;
		}
	}
}