#include "f3d.h"

static std::unique_ptr<f3d::Firestone>	g_f3d = nullptr;

namespace f3d {
	Firestone	*getF3D(void) {
		if (g_f3d.get() == nullptr)
			g_f3d.reset(new Firestone());

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

	Firestone::Firestone() : settings(new f3d::core::Settings()) {
		_run = false;
		_start = nullptr; _start_arg = false;
		_end = nullptr; _end_arg = false;
		_input = nullptr; _input_arg = false;
		_draw = nullptr; _draw_arg = false;
		_resize = nullptr; _resize_arg = false;
	}

	bool	Firestone::execute() {
		
		if (glfwInit() == GLFW_FALSE)
			return false;
		if (glfwVulkanSupported() == GLFW_FALSE)
			return false;

		window.reset(new f3d::core::Window(settings));

		if (_start != nullptr)
			_start(*this, _start_arg);
		_run = true;
	
		while (_run == true) {

		}
		
		

		if (_end != nullptr)
			_end(*this, _end_arg);

		window.reset();
		glfwTerminate();
		return true;
	}

	bool	Firestone::applySettings() {
		window->applySettings();
		return false;
	}
}