#pragma once

#ifndef _F3D_FIRESTONE_IMPL_H
#define _F3D_FIRESTONE_IMPL_H

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

#include "f3d.h"


namespace f3d {
	class FirestoneImpl : public Firestone {
	public:
		FirestoneImpl();
		~FirestoneImpl();

		void	initVK();
		bool	execute();
		bool	applySettings();

	public:
		VkInstance								vk_instance;
	};
}

#endif