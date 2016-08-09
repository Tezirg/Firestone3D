#include "window.h"

namespace f3d {
	namespace core {
		
		Window::Window(std::shared_ptr<f3d::core::Settings>& settingsPtr) : _settings(settingsPtr) {
			glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
			GLFWwindow* window = glfwCreateWindow(_settings->windowWidth, _settings->windowHeight, _settings->windowTitle.c_str(), NULL, NULL);
		}
		Window::~Window() {
		}

	}
}

