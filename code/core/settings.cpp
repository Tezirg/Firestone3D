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
			fullScreen = false;
			fpsCap = 30;
			vSync = false;
		}
	}
}