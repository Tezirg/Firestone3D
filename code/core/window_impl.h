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
			WindowImpl(VkInstance instance, std::shared_ptr<f3d::core::Settings>& settingsPtr);
			~WindowImpl();
			void			applySettings(void);

			GLFWwindow*		getGLFWwindow();
			VkSurfaceKHR	getVulkanSurface();
		private:
			std::shared_ptr<f3d::core::Settings>	_settings;
			GLFWmonitor								*_monitor;
			const GLFWvidmode						*_videoMode;
			GLFWwindow								*_window;
			VkInstance								_vk_instance;
			VkSurfaceKHR							_vk_surface;
		};
	}
}

#endif