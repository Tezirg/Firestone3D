#include <vulkan/vulkan.h>
#include <glfw/glfw3.h>

#include "core/firestone.h"
#include "core/firestone_impl.h"

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

static void			defaultKeyInputCallback(f3d::Firestone& f3d, f3d::utils::KeyInput& keyEvent) {
	
	if (keyEvent.keycode == 256 && keyEvent.state == keyEvent.F3D_KEY_STATE_PRESS)
		f3d.stop();
}

static void			defaultResizeCallback(f3d::Firestone& f3d, int w, int h) {
	f3d.settings->windowWidth = w;
	f3d.settings->windowHeight = h;
	f3d.applySettings();
}

namespace f3d {
	Firestone::Firestone() : settings(nullptr), window(nullptr), renderer(nullptr) {
		_run = false;
		_start = nullptr;
		_end = nullptr;
		_draw = nullptr;
		_resize = defaultResizeCallback;
		_key_input = defaultKeyInputCallback;
		_joystick_input = nullptr; 
		_mouse_input = nullptr;
	}

	Firestone::~Firestone() {
	}

	void	Firestone::startCallback(f3d_start_handle_t handle) {
		_start = handle;
	}

	void	Firestone::endCallback(f3d_end_handle_t handle) {
		_end = handle;
	}

	void	Firestone::resizeCallback(f3d_resize_handle_t handle) {
		_resize = handle;
	}

	void	Firestone::drawCallback(f3d_draw_handle_t handle) {
		_draw = handle;
	}

	void	Firestone::keybordEventsCallback(f3d_keyboard_input_handle_t handle) {
		_key_input = handle;
	}

	void	Firestone::mouseEventsCallback(f3d_mouse_input_handle_t handle) {
		_mouse_input = handle;
	}

	void	Firestone::joystickEventsCallback(f3d_joystick_input_handle_t handle) {
		_joystick_input = handle;
	}
}