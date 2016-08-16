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

static void			defaultInputCallback(f3d::Firestone& f3d, void *arg) {
	(void)arg;
	return;
}

static void			defaultResizeCallback(f3d::Firestone& f3d, int w, int h, void *arg) {
	(void)arg;
	f3d.settings->windowWidth = w;
	f3d.settings->windowHeight = h;
	f3d.applySettings();
}

namespace f3d {
	Firestone::Firestone() : settings(nullptr), window(nullptr), renderer(nullptr) {
		_run = false;
		_start = nullptr; _start_arg = false;
		_end = nullptr; _end_arg = false;
		_input = defaultInputCallback; _input_arg = false;
		_draw = nullptr; _draw_arg = false;
		_resize = defaultResizeCallback; _resize_arg = false;
	}

	Firestone::~Firestone() {
	}

	void	Firestone::startCallback(f3d_start_handle_t handle, void *arg) {
		_start = handle;
		_start_arg = arg;
	}

	void	Firestone::endCallback(f3d_end_handle_t handle, void *arg) {
		_end = handle;
		_end_arg = arg;
	}

	void	Firestone::resizeCallback(f3d_resize_handle_t handle, void *arg) {
		_resize = handle;
		_resize_arg = arg;
	}

	void	Firestone::drawCallback(f3d_draw_handle_t handle, void *arg) {
		_draw = handle;
		_draw_arg = arg;
	}

	void	Firestone::inputCallback(f3d_input_handle_t handle, void *arg) {
		_input = handle;
		_input_arg = arg;

	}
}