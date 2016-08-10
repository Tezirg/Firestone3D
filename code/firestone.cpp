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

	VkInstance		Firestone::getVulkanInstance() {
		return _vk_instance;
	}


	bool	Firestone::execute() {
		
		if (glfwInit() == GLFW_FALSE)
			F3D_FATAL_ERROR("GLFW init failed");
		if (glfwVulkanSupported() == GLFW_FALSE)
			F3D_FATAL_ERROR("No Vulkan installation found");

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

			result = vkCreateInstance(&info, NULL, &_vk_instance);
			F3D_ASSERT_VK(result, VK_SUCCESS, "Failed to create a Vulkan instance");
			F3D_ASSERT(f3d::utils::queryInstancePFN(_vk_instance), "Failed to retreive Vulkan instance functions");
		}

		window.reset(new f3d::core::Window(_vk_instance, settings));
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
		if (window != nullptr)
			window->applySettings();
		
		return false;
	}
}