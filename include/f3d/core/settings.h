#pragma once

#ifndef __F3D_SETTINGS_H
#define __F3D_SETTINGS_H

#include <memory>
#include <cstdint>
#include <iostream>

#include "core/platform.h"

namespace f3d {
	namespace core {
		class F3D_API Settings {
			public:
				Settings();
				~Settings();

				void			default();

				std::string		applicationName;
				uint32_t		applicationVersion;
				uint32_t		windowWidth;
				uint32_t		windowHeight;
				glm::vec4		windowClearColor;
				bool			fullScreen;
				uint32_t		fpsCap;
				bool			vSync;
		};
	}
}

#endif