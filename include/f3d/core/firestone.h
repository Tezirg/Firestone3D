#pragma once

#ifndef _F3D_FIRESTONE_H
#define _F3D_FIRESTONE_H

#include <memory>
#include <cstdint>
#include <thread>
#include <chrono> 

#include "core/plateform.h"
#include "core/settings.h"
#include "core/window.h"
#include "tree/scene.h"
#include "core/renderer.h"
#include "utils/inputs.h"

namespace f3d {

	class			F3D_API Firestone;
	Firestone		F3D_API *getF3D();
	bool			F3D_API releaseF3D();


	// Engine events
	typedef void(*f3d_start_handle_t)(Firestone& f3d, void *userArgument);
	typedef void(*f3d_end_handle_t)(Firestone& f3d, void *userArgument);
	typedef void(*f3d_draw_handle_t)(Firestone& f3d, void *userArgument);

	// Window callbacks
	typedef void(*f3d_resize_handle_t)(Firestone& f3d, int32_t width, int32_t height, void *userArgument);

	// Input callback types
	typedef void(*f3d_keyboard_input_handle_t)(Firestone& f3d, f3d::utils::KeyInput& keyEvent, void *userArgument);
	typedef void(*f3d_mouse_input_handle_t)(Firestone& f3d, f3d::utils::MouseInput& mouseEvent, void *userArgument);
	typedef void(*f3d_joystick_input_handle_t)(Firestone& f3d, f3d::utils::JoystickInput& joystickEvent, void *userArgument);


	class F3D_API Firestone {
	public:
		Firestone();
		virtual ~Firestone();

		virtual bool	execute() = 0;
		virtual bool	applySettings() = 0;
		virtual bool	stop() = 0;
	public:
		void	startCallback(f3d_start_handle_t, void *arg);
		void	endCallback(f3d_end_handle_t, void *arg);
		void	resizeCallback(f3d_resize_handle_t, void *arg);
		void	drawCallback(f3d_draw_handle_t, void *arg);
		void	keybordEventsCallback(f3d_keyboard_input_handle_t, void *arg);
		void	mouseEventsCallback(f3d_mouse_input_handle_t, void *arg);
		void	joystickEventsCallback(f3d_joystick_input_handle_t, void *arg);
	
	public:
		std::shared_ptr<f3d::core::Settings>	settings;
		std::shared_ptr<f3d::core::Window>		window;
		std::shared_ptr<f3d::tree::Scene>		scene;
		std::shared_ptr<f3d::core::Renderer>	renderer;

		f3d_start_handle_t						_start;
		void									*_start_arg;
		f3d_end_handle_t						_end;
		void									*_end_arg;
		f3d_resize_handle_t						_resize;
		void									*_resize_arg;
		f3d_draw_handle_t						_draw;
		void									*_draw_arg;
		f3d_keyboard_input_handle_t				_key_input;
		void									*_key_input_arg;
		f3d_mouse_input_handle_t				_mouse_input;
		void									*_mouse_input_arg;
		f3d_joystick_input_handle_t				_joystick_input;
		void									*_joystick_input_arg;

	protected: //Callbacks 
		bool									_run;
	};

}

#endif