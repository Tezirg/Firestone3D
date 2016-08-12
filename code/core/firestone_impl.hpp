#pragma once

#ifndef F3D_FIRESTONE_PIMPL_H
#define F3D_FIRESTONE_PIMPL_H

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

#include "f3d.h"

namespace f3d {
	class FirestoneImpl : public Firestone {
	public:
		FirestoneImpl() : settings(new f3d::core::Settings()) {
		}
		~FirestoneImpl() {

		}

		void	initVK()
		{ //Init Vulkan instance
			VkResult				result;
			VkApplicationInfo		app_info;
			VkInstanceCreateInfo	info;
			uint32_t				count;
			const char**			extensions = NULL;

			std::memset(&app_info, 0, sizeof(VkApplicationInfo));
			app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
			app_info.pEngineName = F3D_NAME;
			app_info.engineVersion = F3D_VERSION;
			app_info.pApplicationName = settings->applicationName.c_str();
			app_info.applicationVersion = settings->applicationVersion;

			extensions = glfwGetRequiredInstanceExtensions(&count);
			std::memset(&info, 0, sizeof(VkInstanceCreateInfo));
			info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
			info.enabledExtensionCount = count;
			info.ppEnabledExtensionNames = extensions;
			info.pApplicationInfo = &app_info;

			result = vkCreateInstance(&info, NULL, &vk_instance);
			F3D_ASSERT_VK(result, VK_SUCCESS, "Failed to create a Vulkan instance");
			F3D_ASSERT(f3d::utils::queryInstancePFN(vk_instance), "Failed to retreive Vulkan instance functions");
		}

		bool	FirestoneImpl::execute() {

			if (glfwInit() == GLFW_FALSE)
				F3D_FATAL_ERROR("GLFW init failed");
			if (glfwVulkanSupported() == GLFW_FALSE)
				F3D_FATAL_ERROR("No Vulkan installation found");

			initVK();

			window.reset(new f3d::core::Window(vk_instance, settings));
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

		bool	FirestoneImpl::applySettings() {
			if (window != nullptr)
				window->applySettings();

			return false;
		}

	public:
		VkInstance								vk_instance;
		std::shared_ptr<f3d::core::Settings>	settings;
	};
}

#endif