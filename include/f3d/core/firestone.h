#pragma once

#ifndef _F3D_FIRESTONE_H
#define _F3D_FIRESTONE_H

#include <memory>
#include <cstdint>
#include <thread>
#include <chrono> 

#include "core/platform.h"
#include "core/types.h"
#include "core/settings.h"
#include "core/window.h"
#include "tree/scene.h"
#include "core/renderer.h"
#include "utils/inputs.h"

namespace f3d {

	class			F3D_API Firestone;

	/**
	 * @fn getF3D
	 * @desc This function instanciate and initialize the F3D library.
	 * Before most F3D features can be used, F3D need to be instanciated,
	 * and before an application terminates F3D should be cleaned up
	 * in order to free any resources allocated during or after initialization.
	 * @return A pointer to the main F3D engine class, nullptr on error
	 */
	F3D_API Firestone*		getF3D();

	/**
	 * @fn releaseF3D
	 * @desc This function release the internal reference to the main F3D engine,
	 * destroys remaining window and frees any other allocated internal resources.
	 * All references on the main F3D engine are no longer valid.
	 * Once this funciton is called you must call getF3D again to retreive a valid F3D engine instance.
	 * @return True on success, false if any error occurs
	 */
	F3D_API bool			releaseF3D();


	// Engine events
	typedef std::function<void(f3d::Firestone& f3d)> f3d_start_handle_t;
	typedef std::function<void(f3d::Firestone& f3d)> f3d_end_handle_t;
	typedef std::function<void(f3d::Firestone& f3d)> f3d_draw_handle_t;

	// Window callbacks
	typedef void(*f3d_resize_handle_t)(Firestone& f3d, int32_t width, int32_t height);

	// Input callback types
	typedef std::function<void(Firestone& f3d, f3d::utils::KeyInput& keyEvent)> f3d_keyboard_input_handle_t;
	typedef std::function<void(Firestone& f3d, f3d::utils::MouseInput& mouseEvent)> f3d_mouse_input_handle_t;
	typedef std::function<void(Firestone& f3d, f3d::utils::JoystickInput& joystickEvent)> f3d_joystick_input_handle_t;

	class F3D_API Firestone {
	public:
		Firestone();
		virtual ~Firestone();

		virtual bool	execute() = 0;
		virtual bool	applySettings() = 0;
		virtual bool	stop() = 0;
	public:
		void	startCallback(f3d_start_handle_t);
		void	endCallback(f3d_end_handle_t);
		void	resizeCallback(f3d_resize_handle_t);
		void	drawCallback(f3d_draw_handle_t);
		void	keybordEventsCallback(f3d_keyboard_input_handle_t);
		void	mouseEventsCallback(f3d_mouse_input_handle_t);
		void	joystickEventsCallback(f3d_joystick_input_handle_t);
	
	public:
		std::shared_ptr<f3d::core::Settings>	settings;
		std::shared_ptr<f3d::core::Window>		window;
		std::shared_ptr<f3d::tree::Scene>		scene;
		std::shared_ptr<f3d::core::Renderer>	renderer;

		f3d_start_handle_t						_start;
		f3d_end_handle_t						_end;
		f3d_resize_handle_t						_resize;
		f3d_draw_handle_t						_draw;
		f3d_keyboard_input_handle_t				_key_input;
		f3d_mouse_input_handle_t				_mouse_input;
		f3d_joystick_input_handle_t				_joystick_input;

	protected:
		bool									_run;
	};
}

#endif