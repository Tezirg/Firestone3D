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
		public:
			WindowImpl(VkInstance instance, VkPhysicalDevice physical, std::shared_ptr<f3d::core::Settings>& settingsPtr);
			~WindowImpl();

			void			applySettings(void);

			GLFWwindow*		getGLFWwindow();
		private:
			void			initSurface();
			void			initFormatAndColor();
		public:
			VkInstance								vk_instance;
			VkPhysicalDevice						vk_physical_device;
			VkSurfaceKHR							vk_surface; //Surface
			VkFormat								vk_format; //!< Vulkan format native type
			VkColorSpaceKHR							vk_color_space; //!< Vulkan color space
		private:
			std::shared_ptr<f3d::core::Settings>	_settings;
			GLFWmonitor								*_monitor;
			const GLFWvidmode						*_videoMode;
			GLFWwindow								*_window;

		};
	}
}

#endif