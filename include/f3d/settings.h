#pragma once

#ifndef __F3D_SETTINGS_H
#define __F3D_SETTINGS_H

#include <memory>
#include <cstdint>
#include <iostream>

namespace f3d {
	namespace core {
		class Settings {
			public:
				Settings();
				~Settings();

				void			default();

				std::string		windowTitle;
				uint32_t		windowWidth;
				uint32_t		windowHeight;
				bool			fullScreen;
				uint32_t		fpsCap;
		};
	}
}

#endif