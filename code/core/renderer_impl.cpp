#include "renderer_impl.h"

namespace f3d {
	namespace core {
		RendererImpl::RendererImpl(std::shared_ptr<f3d::core::Settings>& sets,
								   std::shared_ptr<f3d::core::Device>& device, 
								   std::shared_ptr<f3d::core::PhysicalDevice>& phys,
								   std::shared_ptr<f3d::core::Window> &win) 
			: _device(device), _physical(phys), _window(win) {
			settings = sets;

			_renders.insert(std::make_pair(F3D_RENDERPASS_SIMPLE, std::unique_ptr<RenderPass>(new f3d::core::renderpass::SimpleRenderPass(_device, _physical, _window))));
		}

		RendererImpl::~RendererImpl() {

		}

		void	RendererImpl::render(std::shared_ptr<f3d::tree::Scene> scene) {
			_window->swapBuffers();

			//_renders[F3D_RENDERPASS_SIMPLE]->render(0, 0, scene);
		}

		void					RendererImpl::display() {
			VkResult			r;
			uint32_t			fam_idx;
			uint32_t			frame_idx;
			VkPresentInfoKHR	present;
			WindowImpl			*win;


			win = dynamic_cast<WindowImpl *>(_window.get());
			fam_idx = _device->getQueueFamilyIndex(true, VK_QUEUE_GRAPHICS_BIT); 
			frame_idx = win->vk_present_frame;

			std::memset(&present, 0, sizeof(VkPresentInfoKHR));
			present.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
			present.pNext = NULL;
			present.swapchainCount = 1;
			present.pSwapchains = & win->vk_swapchain;
			present.pImageIndices = &frame_idx;
			
			r = f3d::utils::fpQueuePresentKHR(_device->getQueue(fam_idx, 0), &present);
			F3D_ASSERT_VK(r, VK_SUCCESS, "Queue presentation fails");
		}

	}
}