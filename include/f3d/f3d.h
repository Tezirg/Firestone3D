#pragma once

#ifndef __F3D_H
#define __F3D_H

#include <memory>
#include <cstdint>
#include <thread>
#include <chrono> 

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

#include "version.h"
#include "core/settings.h"
#include "core/window.h"
#include "utils/vulkan.h"
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
	public:
		Firestone();

		VkInstance		getVulkanInstance();

		bool	execute();
		bool	applySettings();


	public:
		std::shared_ptr<f3d::core::Settings>	settings;
		std::shared_ptr<f3d::core::Window>		window;
		//			std::shared_ptr<f3d::tree::Scene>		scene;
		//			std::shared_ptr<f3d::core::Timer>		timer;

	private:
		bool									_run;
		VkInstance								_vk_instance;

	private://Callbacks 
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