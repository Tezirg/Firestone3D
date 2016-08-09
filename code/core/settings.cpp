#include "core/settings.h"

namespace f3d {
	namespace core {
		Settings::Settings() {
			default();
		}

		Settings::~Settings() {
		}

		void		Settings::default() {
			windowTitle = "F3D Engine";
			windowWidth = 800;
			windowHeight = 600;
			fullScreen = false;
			fpsCap = 30;
			vSync = false;
		}
	}
}