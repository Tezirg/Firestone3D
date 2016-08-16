#include "core/window.h"

namespace f3d {
	namespace core {
		Window::Window() : _settings(nullptr) {
		}

		Window::~Window() {
		}

		int						Window::width() const {
			if (_settings != nullptr)
				return _settings->windowWidth;
			return 0;
		}

		int						Window::height() const { 
			if (_settings != nullptr)
				return _settings->windowHeight;
			return 0;
		}
	}
}

