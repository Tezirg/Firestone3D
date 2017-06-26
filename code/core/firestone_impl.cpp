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

	//Get the position from last mouse event
	double x = 0.0;
	double y = 0.0;
	if (f3d_impl->mouseEvent.get() != nullptr) {
		x = f3d_impl->mouseEvent->xoffset;
		y = f3d_impl->mouseEvent->yoffset;
	}

	f3d_impl->mouseEvent.reset(new f3d::utils::MouseInput(button, state));


	f3d_impl->mouseEvent->xoffset = x;
	f3d_impl->mouseEvent->yoffset = y;
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

static void										joystick_callback(int joy, int event) {
	f3d::Firestone*								f3d = f3d::getF3D();
	f3d::FirestoneImpl*							f3d_impl = dynamic_cast<f3d::FirestoneImpl*>(f3d);

	f3d_impl->joystickEvent.reset(new f3d::utils::JoystickInput(joy, event == GLFW_CONNECTED ? true : false));
	if (f3d->_joystick_input != nullptr) {
		f3d->_joystick_input(*f3d, *f3d_impl->joystickEvent, f3d->_joystick_input_arg);
	}
}

static void										joystick_update(int joy) {
	f3d::Firestone*								f3d = f3d::getF3D();
	f3d::FirestoneImpl*							f3d_impl = dynamic_cast<f3d::FirestoneImpl*>(f3d);
	int											count = 0;
	const float *								axis = nullptr;
	const unsigned char *						buttons = nullptr;

	if (glfwJoystickPresent(joy) == GLFW_FALSE)
		return;

	//Update axis status
	axis = glfwGetJoystickAxes(joy, &count);
	for (int32_t i = 0; i < count && axis != nullptr; i++) {
		f3d_impl->joystickEvent.reset(new f3d::utils::JoystickInput(joy, i, axis[i]));
		if (f3d->_joystick_input != nullptr) { //Emit one event per axis
			f3d->_joystick_input(*f3d, *f3d_impl->joystickEvent, f3d->_joystick_input_arg);
		}
	}

	count = 0;
	buttons = glfwGetJoystickButtons(joy, &count);
	//Update buttons status
	for (int32_t i = 0; i < count && buttons != nullptr; i++) {
		//Compute button state, defaults to Undefined
		f3d::utils::JoystickInput::eJoystickButtonState state = f3d::utils::JoystickInput::eJoystickButtonState::F3D_JOYSTICK_BUTTON_UNDEFINED;
		state = (buttons[i] == GLFW_PRESS) ? f3d::utils::JoystickInput::eJoystickButtonState::F3D_JOYSTICK_BUTTON_PRESS : state;
		state = (buttons[i] == GLFW_RELEASE) ? f3d::utils::JoystickInput::eJoystickButtonState::F3D_JOYSTICK_BUTTON_RELEASE : state;

		f3d_impl->joystickEvent.reset(new f3d::utils::JoystickInput(joy, i, state));
		if (f3d->_joystick_input != nullptr) { //Emit one event per button
			f3d->_joystick_input(*f3d, *f3d_impl->joystickEvent, f3d->_joystick_input_arg);
		}
	}

}

static void					size_callback(GLFWwindow *window, int width, int height) {
	f3d::Firestone*			f3d = f3d::getF3D();
	f3d::FirestoneImpl*		f3d_impl = dynamic_cast<f3d::FirestoneImpl*>(f3d);
	
	f3d_impl->settings->windowHeight = height;
	f3d_impl->settings->windowWidth = width;

	f3d_impl->applySettings();
	// Call resize callback if defined
	if (f3d->_resize != nullptr) {
		f3d->_resize(*f3d, width, height, f3d->_resize_arg);
	}
}

namespace f3d {

	FirestoneImpl::FirestoneImpl() {
		settings.reset(new f3d::core::Settings());
		timer.reset(new f3d::utils::Timer());
		joystickTimer.reset(new f3d::utils::Timer());
	}

	FirestoneImpl::~FirestoneImpl() {
		std::cout << "Destructor: " << __FILE__ << std::endl;
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
		glfwSetJoystickCallback(joystick_callback);

		if (_start != nullptr)
			_start(*this, _start_arg);

		_run = true;
		timer->start();
		joystickTimer->start();
		render->computeCommandBuffers(scene);

		uint64_t frame_interval = (uint64_t)1e9 / settings->fpsCap; //Compute framerate
		uint64_t joystick_interval = (uint64_t)1e9 / 10; //Update joysticks every 100ms
		while (_run == true) {
			//Event Management
			{
				glfwPollEvents();
				if (joystickTimer->ticks() >= joystick_interval) {
					joystickTimer->restart();
					for (int32_t i = 0; i < 2 /*GLFW_JOYSTICK_LAST*/; i++) //@todo: Change Hardcoded number of supported joysticks
						joystick_update(i);
				}
			}

			//Rendering and timer
			if (timer->ticks() >= frame_interval) {

				renderer->render(scene);
				renderer->display();

				timer->restart();
				if (_draw != nullptr) {
					_draw(*this, _draw_arg);
					render->computeCommandBuffers(scene);
				}
			}
			_run = (glfwWindowShouldClose(win->getGLFWwindow()) == GLFW_FALSE);
		}

		if (_end != nullptr) {
			_end(*this, _end_arg);
		}

		vkDeviceWaitIdle(device->vk_device);

		scene.reset();
		renderer.reset();
		window.reset();
		device.reset();
		gpu.reset();

		glfwTerminate();
		
		return true;
	}

	bool	FirestoneImpl::applySettings() {
		if (window != nullptr) {
			window->applySettings();
			vkDeviceWaitIdle(device->vk_device);
			f3d::core::RendererImpl		*render = dynamic_cast<f3d::core::RendererImpl*>(renderer.get());
			render->reset();
		}
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