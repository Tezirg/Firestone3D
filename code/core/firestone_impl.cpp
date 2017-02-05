#include "firestone_impl.h"
#include "core/window_impl.h"


static void									key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	f3d::Firestone*							f3d = f3d::getF3D();
	f3d::FirestoneImpl*						f3d_impl = dynamic_cast<f3d::FirestoneImpl*>(f3d);
	f3d::utils::KeyInput::keyModiferFlags	flags = 0x0;
	f3d::utils::KeyInput::eKeyState			state;

	// Get the key state from GLFW
	state = (action == GLFW_PRESS) ? f3d::utils::KeyInput::eKeyState::F3D_KEY_STATE_PRESS : f3d::utils::KeyInput::eKeyState::F3D_KEY_STATE_UNDEFINED;
	state = (action == GLFW_RELEASE) ? f3d::utils::KeyInput::eKeyState::F3D_KEY_STATE_RELEASE : state;
	state = (action == GLFW_REPEAT) ? f3d::utils::KeyInput::eKeyState::F3D_KEY_STATE_REPEAT : state;

	// Compute the active modifiers bitmask from GLFW
	flags |= (mods & GLFW_MOD_SHIFT) ? f3d::utils::KeyInput::eKeyModifiersFlagsBits::F3D_KEY_MOD_SHIFT : f3d::utils::KeyInput::eKeyModifiersFlagsBits::F3D_KEY_MOD_UNDEFINED;
	flags |= (mods & GLFW_MOD_CONTROL) ? f3d::utils::KeyInput::eKeyModifiersFlagsBits::F3D_KEY_MOD_CTRL : f3d::utils::KeyInput::eKeyModifiersFlagsBits::F3D_KEY_MOD_UNDEFINED;
	flags |= (mods & GLFW_MOD_ALT) ? f3d::utils::KeyInput::eKeyModifiersFlagsBits::F3D_KEY_MOD_ALT : f3d::utils::KeyInput::eKeyModifiersFlagsBits::F3D_KEY_MOD_UNDEFINED;
	flags |= (mods & GLFW_MOD_SUPER) ? f3d::utils::KeyInput::eKeyModifiersFlagsBits::F3D_KEY_MOD_SUPER : f3d::utils::KeyInput::eKeyModifiersFlagsBits::F3D_KEY_MOD_UNDEFINED;
	
	f3d_impl->keyEvent.reset(new f3d::utils::KeyInput(scancode, state, flags));

	// Call keyboard input callback if defined
	if (f3d->_key_input != nullptr) {
		f3d->_key_input(*f3d, * f3d_impl->keyEvent.get(), f3d->_key_input_arg);
	}
}

static void									mouse_pos_callback(GLFWwindow* window, double xpos, double ypos) {
	f3d::Firestone*							f3d = f3d::getF3D();
	f3d::FirestoneImpl*						f3d_impl = dynamic_cast<f3d::FirestoneImpl*>(f3d);

	f3d_impl->mouseEvent.reset(new f3d::utils::MouseInput(xpos, ypos));

	// Call mouse input callback if defined
	if (f3d->_mouse_input != nullptr) {
		f3d->_mouse_input(*f3d,  * f3d_impl->mouseEvent.get(), f3d->_mouse_input_arg);
	}
}

static void									mouse_focus_callback(GLFWwindow* window, int entered) {
	f3d::Firestone*							f3d = f3d::getF3D();
	f3d::FirestoneImpl*						f3d_impl = dynamic_cast<f3d::FirestoneImpl*>(f3d);

	f3d_impl->mouseEvent.reset(new f3d::utils::MouseInput(entered ? true : false));
	
	
	// Call mouse input callback if defined
	if (f3d->_mouse_input != nullptr) {
		f3d->_mouse_input(*f3d,  * f3d_impl->mouseEvent.get(), f3d->_mouse_input_arg);
	}
}

static void										mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	f3d::Firestone*								f3d = f3d::getF3D();
	f3d::FirestoneImpl*							f3d_impl = dynamic_cast<f3d::FirestoneImpl*>(f3d);
	f3d::utils::MouseInput::eMouseButtonState	state;

	// Get the mouse button state from GLFW
	state = (action == GLFW_PRESS) ? f3d::utils::MouseInput::eMouseButtonState::F3D_MOUSE_BUTTON_PRESS : f3d::utils::MouseInput::eMouseButtonState::F3D_MOUSE_BUTTON_UNDEFINED;
	state = (action == GLFW_RELEASE) ? f3d::utils::MouseInput::eMouseButtonState::F3D_MOUSE_BUTTON_RELEASE : state;
	state = (action == GLFW_REPEAT) ? f3d::utils::MouseInput::eMouseButtonState::F3D_MOUSE_BUTTON_REPEAT : state;

	f3d_impl->mouseEvent.reset(new f3d::utils::MouseInput(button, state));
	
	// Call mouse input callback if defined
	if (f3d->_mouse_input != nullptr) {
		f3d->_mouse_input(*f3d, * f3d_impl->mouseEvent.get(), f3d->_mouse_input_arg);
	}
}

static void										mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	f3d::Firestone*								f3d = f3d::getF3D();
	f3d::FirestoneImpl*							f3d_impl = dynamic_cast<f3d::FirestoneImpl*>(f3d);

	f3d_impl->mouseEvent.reset(new f3d::utils::MouseInput(yoffset));

	// Call mouse input callback if defined
	if (f3d->_mouse_input != nullptr) {
		f3d->_mouse_input(*f3d, *f3d_impl->mouseEvent.get(), f3d->_mouse_input_arg);
	}
}

static void					size_callback(GLFWwindow *window, int width, int height) {
	f3d::Firestone*			f3d = f3d::getF3D();

	// Call resize callback if defined
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
		const char**			extensions;

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
		f3d::core::RendererImpl		*render = nullptr;

		if (glfwInit() == GLFW_FALSE)
			F3D_FATAL_ERROR("GLFW init failed");
		if (glfwVulkanSupported() == GLFW_FALSE)
			F3D_FATAL_ERROR("No Vulkan installation found");

		initVkInstance();
		gpu.reset(new f3d::core::PhysicalDevice(vk_instance));
		device.reset(new f3d::core::Device(vk_instance, gpu->vk_physical_device));
		scene.reset(new f3d::tree::SceneImpl(gpu, device));
		win = new f3d::core::WindowImpl(vk_instance, gpu->vk_physical_device, device, settings);
		window.reset(win);
		render = new f3d::core::RendererImpl(settings, device, gpu, window);
		renderer.reset(render);
		
		//Setup firestone events inner callbacks
		glfwSetKeyCallback(win->getGLFWwindow(), key_callback);
		glfwSetWindowSizeCallback(win->getGLFWwindow(), size_callback);
		glfwSetCursorPosCallback(win->getGLFWwindow(), mouse_pos_callback);
		glfwSetCursorEnterCallback(win->getGLFWwindow(), mouse_focus_callback);
		glfwSetMouseButtonCallback(win->getGLFWwindow(), mouse_button_callback);
		glfwSetScrollCallback(win->getGLFWwindow(), mouse_scroll_callback);

		if (_start != nullptr)
			_start(*this, _start_arg);

		_run = true;
		timer->start();
		render->computeCommandBuffers(scene);
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

	bool	FirestoneImpl::stop() {
		f3d::core::WindowImpl		*win = nullptr;

		win = dynamic_cast<f3d::core::WindowImpl *>(window.get());
		if (win != nullptr) {
			_run = false;
			glfwSetWindowShouldClose(win->getGLFWwindow(), GLFW_TRUE);
			return true;
		}
		return false;
	}

}