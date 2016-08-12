#pragma once

#ifndef _F3D_FIRESTONE_H
#define _F3D_FIRESTONE_H

#include <memory>
#include <cstdint>
#include <thread>
#include <chrono> 

#include "f3d.h"

#include "version.h"
#include "core/settings.h"
#include "core/window.h"
#include "tree/scene.h"
#include "tree/mesh.h"
#include "tree/light.h"
#include "utils/logging.h"


namespace f3d {

	class Firestone;
	Firestone	*getF3D();
	bool		releaseF3D();


	typedef void(*f3d_start_handle_t)(Firestone& f3d, void *argument);
	typedef void(*f3d_end_handle_t)(Firestone& f3d, void *argument);
	typedef void(*f3d_resize_handle_t)(Firestone& f3d, int32_t width, int32_t height, void *argument);
	typedef void(*f3d_draw_handle_t)(Firestone& f3d, void *argument);
	typedef void(*f3d_input_handle_t)(Firestone& f3d, void *argument);

	class Firestone {
	protected:
		Firestone();
	public:
		virtual ~Firestone();

		virtual bool	execute() = 0;
		virtual bool	applySettings() = 0;

	public:
		std::shared_ptr<f3d::core::Settings>	settings;
		std::shared_ptr<f3d::core::Window>		window;
		//std::shared_ptr<f3d::tree::Scene>		scene;
		//			std::shared_ptr<f3d::core::Timer>		timer;

	protected: //Callbacks 
		bool									_run;
		f3d_start_handle_t						_start;
		void									*_start_arg;
		f3d_end_handle_t						_end;
		void									*_end_arg;
		f3d_resize_handle_t						_resize;
		void									*_resize_arg;
		f3d_draw_handle_t						_draw;
		void									*_draw_arg;
		f3d_input_handle_t						_input;
		void									*_input_arg;
	};

}

#endif