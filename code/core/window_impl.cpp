#include "window_impl.h"

namespace f3d {
	namespace core {
		
		WindowImpl::WindowImpl(VkInstance instance, VkPhysicalDevice physical, std::shared_ptr<f3d::core::Settings>& settingsPtr) 
			: vk_instance(instance), vk_physical_device(physical), _settings(settingsPtr), vk_surface((VkSurfaceKHR)0), _window(nullptr)
		{
			_monitor = glfwGetPrimaryMonitor();
			_videoMode = glfwGetVideoMode(_monitor);
			glfwWindowHint(GLFW_REFRESH_RATE, _videoMode->refreshRate);
			glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

			_window = glfwCreateWindow(_settings->windowWidth, _settings->windowHeight, _settings->applicationName.c_str(), NULL, NULL);
			applySettings();
		}

		WindowImpl::~WindowImpl() {
			if (vk_surface != 0)
				vkDestroySurfaceKHR(vk_instance, vk_surface, NULL);

			if (_window != nullptr && _window != NULL)
				glfwDestroyWindow(_window);
		}

		void				WindowImpl::applySettings() {
			GLFWmonitor*	updateWindowMonitor = NULL;

			if (_settings->fullScreen == true) {
				updateWindowMonitor = _monitor;
			}
			if (_settings->vSync == true) {
				_settings->fpsCap = _videoMode->refreshRate;
			}
			//Special case for "Windowed Fullscreen mode"
			if (_videoMode->width == _settings->windowWidth &&
				_videoMode->height == _settings->windowHeight) {
				updateWindowMonitor = _monitor;
			}
			glfwSetWindowMonitor(_window, updateWindowMonitor, 100, 100, _settings->windowWidth, _settings->windowHeight, GLFW_DONT_CARE);

			//Destroy old surface if exists
			if (vk_surface != 0)
				vkDestroySurfaceKHR(vk_instance, vk_surface, NULL);
			vk_surface = (VkSurfaceKHR)0;
			initSurface();
			initFormatAndColor();
		}

		GLFWwindow*		WindowImpl::getGLFWwindow() {
			return		_window;
		}

		void			WindowImpl::initSurface() {
			VkResult res = glfwCreateWindowSurface(vk_instance, _window, 0, &vk_surface);
			F3D_ASSERT_VK(res, VK_SUCCESS, "Surface KHR creation");
		}

		void			WindowImpl::initFormatAndColor() {
			VkResult					res;
			uint32_t					formatCount = 0;
			VkSurfaceFormatKHR			*surfFormats = 0;

			res = f3d::utils::fpGetPhysicalDeviceSurfaceFormatsKHR(vk_physical_device, vk_surface, &formatCount, NULL);
			F3D_ASSERT_VK(res, VK_SUCCESS, "Get surface format count");

			surfFormats = new VkSurfaceFormatKHR[formatCount];
			res = f3d::utils::fpGetPhysicalDeviceSurfaceFormatsKHR(vk_physical_device, vk_surface, &formatCount, surfFormats);
			F3D_ASSERT_VK(res, VK_SUCCESS, "Get surface formats failed");

			// If the format list includes just one entry of VK_FORMAT_UNDEFINED,
			// the surface has no preferred format.  Otherwise, at least one
			// supported format will be returned.
			if (formatCount == 1 && surfFormats[0].format == VK_FORMAT_UNDEFINED) {
				vk_format = VK_FORMAT_B8G8R8A8_UNORM;
			}
			else {
				vk_format = surfFormats[0].format;
			}
			vk_color_space = surfFormats[0].colorSpace;
		}

	}
}

