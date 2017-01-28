#include "renderer_impl.h"

namespace f3d {
	namespace core {
		RendererImpl::RendererImpl(std::shared_ptr<f3d::core::Settings>& sets,
								   std::shared_ptr<f3d::core::Device>& device, 
								   std::shared_ptr<f3d::core::PhysicalDevice>& phys,
								   std::shared_ptr<f3d::core::Window> &win) 
			: _device(device), _physical(phys), _window(win) {
			settings = sets;

			initCommandBuffers();
			_renders.insert(std::make_pair(F3D_RENDERPASS_SIMPLE, std::unique_ptr<RenderPass>(new f3d::core::renderpass::SimpleRenderPass(_device, _physical, _window))));
		}

		RendererImpl::~RendererImpl() {

		}

		void				RendererImpl::initCommandBuffers() {
			VkResult					r;
			VkCommandBufferAllocateInfo	cmd_info;
			uint32_t					pool_fam_idx;
			WindowImpl		*win = dynamic_cast<WindowImpl *>(_window.get());

			vk_commands = new VkCommandBuffer[win->vk_image_count];
			pool_fam_idx = _device->getQueueFamilyIndex(true, VK_QUEUE_GRAPHICS_BIT, win->vk_surface);
			std::memset(&cmd_info, 0, sizeof(VkCommandBufferAllocateInfo));
			cmd_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
			cmd_info.commandPool = _device->getCommandPool(pool_fam_idx);
			cmd_info.commandBufferCount = win->vk_image_count;
			cmd_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
			r = vkAllocateCommandBuffers(_device->vk_device, &cmd_info, vk_commands);
			F3D_ASSERT_VK(r, VK_SUCCESS, "Allocating swapchain command buffers failed");
		}

		void				RendererImpl::computeCommandBuffers(std::shared_ptr<f3d::tree::Scene> scene) {
			WindowImpl		*win = dynamic_cast<WindowImpl *>(_window.get());

			for (uint32_t i = 0; i < win->vk_image_count; i++) {
				win->vk_present_frame = i;
				_renders[F3D_RENDERPASS_SIMPLE]->render(vk_commands[i], scene);
			}
			win->vk_present_frame = 0;
		}

		void						RendererImpl::render(std::shared_ptr<f3d::tree::Scene> scene) {
			VkResult				r;
			WindowImpl				*win = dynamic_cast<WindowImpl *>(_window.get());
			VkSemaphoreCreateInfo	semaphoreInfo;
			uint32_t				fam_idx = _device->getQueueFamilyIndex(true, VK_QUEUE_GRAPHICS_BIT, win->vk_surface);

			win->swapBuffers();

			_renders[F3D_RENDERPASS_SIMPLE]->render(vk_commands[win->vk_present_frame], scene);

			semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
			semaphoreInfo.pNext = 0;
			semaphoreInfo.flags = 0;
			r = vkCreateSemaphore(_device->vk_device, &semaphoreInfo, NULL, &vk_render_semaphore);
			F3D_ASSERT_VK(r, VK_SUCCESS, "Creating render semaphore fails");

			r = vkDeviceWaitIdle(_device->vk_device);
			F3D_ASSERT_VK(r, VK_SUCCESS, "Wait device IDLE");

			VkPipelineStageFlags pipe_stage_flags = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
			VkSubmitInfo submit_info;
			submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
			submit_info.pNext = NULL;
			submit_info.waitSemaphoreCount = 1;
			submit_info.pWaitSemaphores = &win->vk_presentation_semaphore;
			submit_info.pWaitDstStageMask = &pipe_stage_flags;
			submit_info.commandBufferCount = 1;
			submit_info.pCommandBuffers = & vk_commands[win->vk_present_frame];
			submit_info.signalSemaphoreCount = 1;
			submit_info.pSignalSemaphores = &vk_render_semaphore;
			r = vkQueueSubmit(_device->getQueue(fam_idx, 0), 1, &submit_info, VK_NULL_HANDLE);
			F3D_ASSERT_VK(r, VK_SUCCESS, "Submit to Queue");
		}

		void					RendererImpl::display() {
			VkResult			r;
			uint32_t			fam_idx;
			uint32_t			frame_idx;
			VkPresentInfoKHR	present;
			WindowImpl			*win;


			win = dynamic_cast<WindowImpl *>(_window.get());
			fam_idx = _device->getQueueFamilyIndex(true, VK_QUEUE_GRAPHICS_BIT, win->vk_surface); 
			frame_idx = win->vk_present_frame;

			std::memset(&present, 0, sizeof(VkPresentInfoKHR));
			present.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
			present.pNext = NULL;
			present.swapchainCount = 1;
			present.pSwapchains = & win->vk_swapchain;
			present.pImageIndices = &frame_idx;
			present.waitSemaphoreCount = 1;
			present.pWaitSemaphores = &vk_render_semaphore;
			
			r = f3d::utils::fpQueuePresentKHR(_device->getQueue(fam_idx, 0), &present);
			F3D_ASSERT_VK(r, VK_SUCCESS, "Queue presentation fails");

			r = vkQueueWaitIdle(_device->getQueue(fam_idx, 0));
			F3D_ASSERT_VK(r, VK_SUCCESS, "Wait for queue presentation fails");

			vkDestroySemaphore(_device->vk_device, vk_render_semaphore, NULL);
		}

	}
}