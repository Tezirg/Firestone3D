#include "core/settings.h"

namespace f3d {
	namespace core {
		Settings::Settings() {
			default();
		}

		Settings::~Settings() {
		}

		void		Settings::default() {
			applicationName = "F3D application";
			applicationVersion = (1 << 24);
			windowWidth = 800;
			windowHeight = 600;
			windowClearColor = glm::vec4(0.2, 0.2, 0.2, 1.0);
			fullScreen = false;
			fpsCap = 30;
			vSync = false;
		}
	}
}