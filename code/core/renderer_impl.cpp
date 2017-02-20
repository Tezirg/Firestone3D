#include "renderer_impl.h"

namespace f3d {
	namespace core {
		RendererImpl::RendererImpl(std::shared_ptr<f3d::core::Settings>& sets,
								   std::shared_ptr<f3d::core::Device>& device, 
								   std::shared_ptr<f3d::core::PhysicalDevice>& phys,
								   std::shared_ptr<f3d::core::Window> &win) 
			: _device(device), _physical(phys), _window(win) {
			settings = sets;
			
			vk_command_count = 0;

			initCommandBuffers();
			_renders.insert(std::make_pair(f3d::core::RenderPass::F3D_RENDERPASS_SIMPLE, new f3d::core::renderpass::SimpleRenderPass(_device, _physical, _window)));
		}

		RendererImpl::~RendererImpl() {
			std::cout << "Destructor: " << __FILE__ << std::endl;

			if (vk_commands != nullptr) {
				vkFreeCommandBuffers(_device->vk_device, vk_command_pool, vk_command_count, vk_commands);
				delete[] vk_commands;
			}

			std::cout << "Destructor end: " << __FILE__ << std::endl;
		}

		void								RendererImpl::initCommandBuffers() {
			VkResult						r;
			VkCommandBufferAllocateInfo		cmd_info;
			uint32_t						pool_fam_idx;
			WindowImpl						*win = dynamic_cast<WindowImpl *>(_window.get());

			vk_command_count = win->vk_image_count;
			vk_commands = new VkCommandBuffer[vk_command_count];
			
			pool_fam_idx = _device->getQueueFamilyIndex(true, VK_QUEUE_GRAPHICS_BIT, win->vk_surface); 
			vk_command_pool = _device->getCommandPool(pool_fam_idx);
			
			std::memset(&cmd_info, 0, sizeof(VkCommandBufferAllocateInfo));
			cmd_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
			cmd_info.commandPool = vk_command_pool;
			cmd_info.commandBufferCount = vk_command_count;
			cmd_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
			r = vkAllocateCommandBuffers(_device->vk_device, &cmd_info, vk_commands);
			F3D_ASSERT_VK(r, VK_SUCCESS, "Allocating swapchain command buffers failed");
		}

		void							RendererImpl::computeCommandBuffers(std::shared_ptr<f3d::tree::Scene> scene) {
			WindowImpl *				win = dynamic_cast<WindowImpl *>(_window.get());
			f3d::tree::CameraImpl *		cam = dynamic_cast<f3d::tree::CameraImpl *>(scene->getCamera().get());
			f3d::tree::TextureImpl *	texture = nullptr;

			cam->updateDescriptorSet();
			for (auto it = scene->getMaterials().begin(); it != scene->getMaterials().end(); ++it) {
				if ((*it)->getTextures().empty() == false) {
					texture = dynamic_cast<f3d::tree::TextureImpl *>((*it)->getTextures().front());
					texture->updateDescriptorSet();
				}
			}

			for (uint32_t i = 0; i < win->vk_image_count; i++) {
				win->vk_present_frame = i;
				_renders[f3d::core::RenderPass::F3D_RENDERPASS_SIMPLE]->render(vk_commands[i], scene);
			}
			win->vk_present_frame = 0;
		}

		void							RendererImpl::render(std::shared_ptr<f3d::tree::Scene> scene) {
			VkResult					r;
			WindowImpl *				win = dynamic_cast<WindowImpl *>(_window.get());
			f3d::tree::CameraImpl *		cam = dynamic_cast<f3d::tree::CameraImpl *>(scene->getCamera().get());
			f3d::tree::TextureImpl *	texture = nullptr;
			uint32_t					fam_idx = _device->getQueueFamilyIndex(true, VK_QUEUE_GRAPHICS_BIT, win->vk_surface);

			std::cout << "Pre swap" << std::endl;
			win->swapBuffers();
			std::cout << "Post swap" << std::endl;

			cam->updateAttribute();
			// /*
			cam->updateDescriptorSet();
			for (auto it = scene->getMaterials().begin(); it != scene->getMaterials().end(); ++it) {
				if ((*it)->getTextures().empty() == false) {
					texture = dynamic_cast<f3d::tree::TextureImpl *>((*it)->getTextures().front());
					texture->updateDescriptorSet();
				}
			}
			_renders[f3d::core::RenderPass::F3D_RENDERPASS_SIMPLE]->render(vk_commands[win->vk_present_frame], scene);
			// */

			std::cout << "Pre render cmd submit" << std::endl;

			VkPipelineStageFlags pipe_stage_flags = VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT;
			VkSubmitInfo submit_info;
			submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
			submit_info.pNext = NULL;
			submit_info.waitSemaphoreCount = 1;
			submit_info.pWaitSemaphores = &win->vk_present_semaphore;
			submit_info.pWaitDstStageMask = &pipe_stage_flags;
			submit_info.commandBufferCount = 1;
			submit_info.pCommandBuffers = & vk_commands[win->vk_present_frame];
			submit_info.signalSemaphoreCount = 1;
			submit_info.pSignalSemaphores = &win->vk_render_semaphore;
			r = vkQueueSubmit(_device->getQueue(fam_idx, 0), 1, &submit_info, VK_NULL_HANDLE);
			F3D_ASSERT_VK(r, VK_SUCCESS, "Submit to Queue");

			std::cout << "Post render cmd submit" << std::endl;
		}

		void					RendererImpl::display() {
			VkResult			r;
			uint32_t			fam_idx;
			VkPresentInfoKHR	present;
			WindowImpl			*win;

			std::cout << "Pre display" << std::endl;

			win = dynamic_cast<WindowImpl *>(_window.get());
			fam_idx = _device->getQueueFamilyIndex(true, VK_QUEUE_GRAPHICS_BIT, win->vk_surface);

			std::memset(&present, 0, sizeof(VkPresentInfoKHR));
			present.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
			present.pNext = NULL;
			present.swapchainCount = 1;
			present.pSwapchains = & win->vk_swapchain;
			present.pImageIndices = &win->vk_present_frame;
			present.waitSemaphoreCount = 1;
			present.pWaitSemaphores = &win->vk_render_semaphore;
			
			r = f3d::utils::fpQueuePresentKHR(_device->getQueue(fam_idx, 0), &present);
			F3D_ASSERT_VK(r, VK_SUCCESS, "Queue presentation fails");

			r = vkQueueWaitIdle(_device->getQueue(fam_idx, 0));
			F3D_ASSERT_VK(r, VK_SUCCESS, "Wait for queue presentation fails");

			r = vkDeviceWaitIdle(_device->vk_device);
			F3D_ASSERT_VK(r, VK_SUCCESS, "Wait device IDLE");

			std::cout << "Post display" << std::endl;
		}

	}
}