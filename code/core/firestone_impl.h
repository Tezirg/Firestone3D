#pragma once

#ifndef _F3D_FIRESTONE_IMPL_H
#define _F3D_FIRESTONE_IMPL_H

#include <vulkan/vulkan.h>
#include <glfw/glfw3.h>

#include "f3d.h"
#include "physical_device.h"
#include "device.h"
#include "window_impl.h"
#include "renderer_impl.h"
#include "utils/inputs.h"

namespace f3d {
	class FirestoneImpl : public Firestone {
	public:
		FirestoneImpl();
		~FirestoneImpl();

		bool	execute();
		bool	applySettings();
		bool	stop();
	private:
		void	initVkInstance();

	public:
		VkInstance									vk_instance;
		std::shared_ptr<f3d::core::WindowImpl>		win; 
		std::shared_ptr<f3d::core::PhysicalDevice>	gpu;
		std::shared_ptr<f3d::core::Device>			device;
		std::shared_ptr<f3d::utils::Timer>			timer;
		std::unique_ptr<f3d::utils::KeyInput>		keyEvent;
		std::unique_ptr<f3d::utils::MouseInput>		mouseEvent;
	};
}

#endif