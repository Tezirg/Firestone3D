#pragma once

#ifndef __F3D_WINDOW_IMPL_H
#define __F3D_WINDOW_IMPL_H

#include <memory>
#include <cstdint>
#include <vulkan/vulkan.h>
#include <glfw/glfw3.h>

#include "core/plateform.h"
#include "utils/vulkan.h"
#include "core/device.h"
#include "core/window.h"
#include "core/physical_device.h"
#include "core/settings.h"

namespace f3d {
	namespace core {
		class F3D_API WindowImpl : public Window {
		//Implements Window interface
		public:
			WindowImpl(VkInstance instance, VkPhysicalDevice physical, std::shared_ptr<f3d::core::Device>& device, std::shared_ptr<f3d::core::Settings>& settingsPtr);
			~WindowImpl();

			void			applySettings(void);
			void			swapBuffers();
		
		//Hidden implementation details
		public:
			GLFWwindow*		getGLFWwindow();
		private:
			void			initSurface();
			void			initFormatAndColor();
			void			initSwapchain();
			void			initImages();
		public:
			//Depdencies vars
			VkInstance								vk_instance;
			VkPhysicalDevice						vk_physical_device;
			VkDevice								vk_device;

			//Window vars
			VkSurfaceKHR							vk_surface; //Surface
			VkFormat								vk_format; //!< Vulkan format native type
			VkColorSpaceKHR							vk_color_space; //!< Vulkan color space

			//Swapchain vars
			VkSwapchainKHR							vk_swapchain; //Swapchain
			uint32_t								vk_image_count; //Swapchain image count
			VkImage*								vk_images; //Swapchain images
			uint32_t								vk_present_frame;//Index of the next presentable in the swapchain
			VkSemaphore								vk_acquire_semaphore; //Semaphore signaled after presentation completes
			VkSemaphore								vk_swap_semaphore; //Semaphore signaled after swap is successfull
			VkSemaphore								vk_render_semaphore; //Semaphore signaled after rendering is complete

		private:
			std::shared_ptr<f3d::core::Device>		_device;
			GLFWmonitor								*_monitor;
			const GLFWvidmode						*_videoMode;
			GLFWwindow								*_window;

		};
	}
}

#endif