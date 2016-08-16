#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

#include "firestone_impl.h"
#include "core/firestone.h"

static std::unique_ptr<f3d::Firestone>	g_f3d = nullptr;

namespace f3d {
	Firestone	*getF3D(void) {
		if (g_f3d.get() == nullptr)
			g_f3d.reset(new FirestoneImpl());

		return g_f3d.get();
	}

	bool		releaseF3D(void) {
		if (g_f3d.get() == nullptr)
			return false;
		g_f3d.reset();
		return true;
	}
}

namespace f3d {
	Firestone::Firestone() : settings(nullptr), window(nullptr), renderer(nullptr) {
		_run = false;
		_start = nullptr; _start_arg = false;
		_end = nullptr; _end_arg = false;
		_input = nullptr; _input_arg = false;
		_draw = nullptr; _draw_arg = false;
		_resize = nullptr; _resize_arg = false;
	}

	Firestone::~Firestone() {
	}

}