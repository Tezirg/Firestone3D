#pragma once

#ifndef __F3D_WINDOW_H
#define __F3D_WINDOW_H

#include <memory>
#include <cstdint>
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

#include "core/settings.h"

namespace f3d {
	namespace core {
		class Window {
		public:
			Window(std::shared_ptr<f3d::core::Settings>& settingsPtr);
			~Window();
			void			applySettings(void);

			GLFWwindow*		getGLFWwindow();
			VkSurfaceKHR	getVulkanSurface();
		private:
			std::shared_ptr<f3d::core::Settings>	_settings;
			GLFWmonitor								*_monitor;
			const GLFWvidmode						*_videoMode;
			GLFWwindow								*_window;
			VkSurfaceKHR							_surface;
		};
	}
}

#endif