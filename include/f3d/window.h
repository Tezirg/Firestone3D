#pragma once

#ifndef __F3D_WINDOW_H
#define __F3D_WINDOW_H

#include <memory>
#include <cstdint>
#include <GLFW/glfw3.h>

#include "settings.h"

namespace f3d {
	namespace core {
		class Window {
		public:
			Window(std::shared_ptr<f3d::core::Settings>& settingsPtr);
			~Window();
		private:
			std::shared_ptr<f3d::core::Settings>	_settings;
		};
	}
}

#endif