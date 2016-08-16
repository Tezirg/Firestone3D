#include "firestone_impl.h"
#include "core/window_impl.h"


static void					key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	f3d::Firestone*			f3d = f3d::getF3D();
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	if (f3d->_input != nullptr) {
		f3d->_input(*f3d, f3d->_input_arg);
	}
}

static void					size_callback(GLFWwindow *window, int width, int height) {
	f3d::Firestone*			f3d = f3d::getF3D();

	if (f3d->_resize != nullptr) {
		f3d->_resize(*f3d, width, height, f3d->_resize_arg);
	}
}


namespace f3d {

	FirestoneImpl::FirestoneImpl() {
		settings.reset(new f3d::core::Settings());
		timer.reset(new f3d::utils::Timer());
	}

	FirestoneImpl::~FirestoneImpl() {
	}

	void	FirestoneImpl::initVkInstance()
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

	bool							FirestoneImpl::execute() {
		f3d::core::WindowImpl		*win = nullptr;

		if (glfwInit() == GLFW_FALSE)
			F3D_FATAL_ERROR("GLFW init failed");
		if (glfwVulkanSupported() == GLFW_FALSE)
			F3D_FATAL_ERROR("No Vulkan installation found");

		initVkInstance();
		gpu.reset(new f3d::core::PhysicalDevice(vk_instance));
		device.reset(new f3d::core::Device(vk_instance, gpu->vk_physical_device));
		win = new f3d::core::WindowImpl(vk_instance, gpu->vk_physical_device, device->vk_device, settings);
		window.reset(win);
		renderer.reset(new f3d::core::RendererImpl(settings, device, window));
		
		glfwSetKeyCallback(win->getGLFWwindow(), key_callback);
		glfwSetWindowSizeCallback(win->getGLFWwindow(), size_callback);

		if (_start != nullptr)
			_start(*this, _start_arg);

		_run = true;
		timer->start();
		while (_run == true) {
			uint64_t fps_nano = (uint64_t)1e9 / settings->fpsCap;
			if (timer->ticks() >= fps_nano) {
				renderer->render(scene);
				renderer->display();
				timer->restart();
				if (_draw != nullptr) {
					_draw(*this, _draw_arg);
				}
			}
			glfwPollEvents();
			_run = (glfwWindowShouldClose(win->getGLFWwindow()) == GLFW_FALSE);
		}

		if (_end != nullptr) {
			_end(*this, _end_arg);
		}

		window.reset();
		glfwTerminate();
		
		return true;
	}

	bool	FirestoneImpl::applySettings() {
		if (window != nullptr)
			window->applySettings();
		return false;
	}
}