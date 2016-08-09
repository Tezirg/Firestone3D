#include "window.h"

namespace f3d {
	namespace core {
		
		Window::Window(std::shared_ptr<f3d::core::Settings>& settingsPtr) 
			: _settings(settingsPtr) 
		{
			GLFWmonitor	*createWindowMonitor = NULL;

			_window = nullptr;
			_monitor = glfwGetPrimaryMonitor();
			_videoMode = glfwGetVideoMode(_monitor);
			glfwWindowHint(GLFW_REFRESH_RATE, _videoMode->refreshRate);
			glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

			if (_settings->fullScreen == true) {
				createWindowMonitor = _monitor;
			}
			if (_settings->vSync == true) {
				_settings->fpsCap = _videoMode->refreshRate;
			}
			//Special case for "Windowed Fullscreen mode"
			if (_videoMode->width == _settings->windowWidth &&
				_videoMode->height == _settings->windowHeight) {
				createWindowMonitor = _monitor;
			}

			_window = glfwCreateWindow(_settings->windowWidth, _settings->windowHeight, _settings->windowTitle.c_str(), createWindowMonitor, NULL);

		}
		Window::~Window() {
			if (_window != nullptr && _window != NULL)
				glfwDestroyWindow(_window);
		}

		void		Window::applySettings() {
			GLFWmonitor	*updateWindowMonitor = NULL;

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
			glfwSetWindowMonitor(_window, updateWindowMonitor, 0, 0, _settings->windowWidth, _settings->windowHeight, GLFW_DONT_CARE);
		}

	}
}

