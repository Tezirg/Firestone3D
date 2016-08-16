#pragma once

#ifndef __F3D_WINDOW_IMPL_H
#define __F3D_WINDOW_IMPL_H

#include <memory>
#include <cstdint>
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

#include "f3d.h"
#include "utils/vulkan.h"

namespace f3d {
	namespace core {
		class WindowImpl : public Window {
		//Implements Window interface
		public:
			WindowImpl(VkInstance instance, VkPhysicalDevice physical, VkDevice device, std::shared_ptr<f3d::core::Settings>& settingsPtr);
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

		private:
			std::shared_ptr<f3d::core::Settings>	_settings;
			GLFWmonitor								*_monitor;
			const GLFWvidmode						*_videoMode;
			GLFWwindow								*_window;

		};
	}
}

#endif