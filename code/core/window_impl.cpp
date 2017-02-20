#include "window_impl.h"

namespace f3d {
	namespace core {
		
		WindowImpl::WindowImpl(VkInstance instance, VkPhysicalDevice physical, 
			std::shared_ptr<f3d::core::Device>& device, std::shared_ptr<f3d::core::Settings>& settingsPtr) 
			: vk_instance(instance), vk_physical_device(physical), vk_device(device->vk_device),
			  vk_surface((VkSurfaceKHR)0), vk_swapchain((VkSwapchainKHR)0),
			  _device(device), _window(nullptr)
		{
			_settings = settingsPtr;
			glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
			_monitor = glfwGetPrimaryMonitor();
			_videoMode = glfwGetVideoMode(_monitor);
			glfwWindowHint(GLFW_REFRESH_RATE, _videoMode->refreshRate);

			vk_present_frame = 0;
			vk_images = nullptr;
			vk_image_count = 0;
			vk_presentation_semaphore = 0;

			_window = glfwCreateWindow(_settings->windowWidth, _settings->windowHeight, _settings->applicationName.c_str(), NULL, NULL);
			applySettings();
		}

		WindowImpl::~WindowImpl() {
			if (vk_swapchain != 0)
				vkDestroySwapchainKHR(vk_device, vk_swapchain, NULL);
			if (vk_surface != 0)
				vkDestroySurfaceKHR(vk_instance, vk_surface, NULL);

			if (_window != nullptr && _window != NULL)
				glfwDestroyWindow(_window);
		}

		void				WindowImpl::applySettings() {
			int				x = 10, y = 10;
			GLFWmonitor*	updateWindowMonitor = NULL;

			glfwGetWindowPos(_window,&x, &y);

			if (_settings->fullScreen == true) {
				updateWindowMonitor = _monitor;
			}
			if (_settings->vSync == true) {
				_settings->fpsCap = _videoMode->refreshRate;
			}
			//Special case for "Windowed Fullscreen mode"
			if (_videoMode->width == _settings->windowWidth &&
				_videoMode->height == _settings->windowHeight) {
				updateWindowMonitor = _monitor;
			}
			glfwSetWindowMonitor(_window, updateWindowMonitor, x, y, _settings->windowWidth, _settings->windowHeight, GLFW_DONT_CARE);

			initSurface();
			initFormatAndColor();
			initSwapchain();
			initImages();
		}

		void							WindowImpl::swapBuffers() {
			VkResult					r;
			VkFence						nullFence = VK_NULL_HANDLE;
			VkSemaphoreCreateInfo		semaphore_infos;

			if (vk_presentation_semaphore != 0) {
				vkDestroySemaphore(_device->vk_device, vk_presentation_semaphore, NULL);
				vk_presentation_semaphore = 0;
			}
			std::memset(&semaphore_infos, 0, sizeof(VkSemaphoreCreateInfo));
			semaphore_infos.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

			r = vkCreateSemaphore(_device->vk_device, &semaphore_infos, NULL, &vk_presentation_semaphore);
			F3D_ASSERT_VK(r, VK_SUCCESS, "Presentation semaphore creation failed");

			// Get the index of the next available swapchain image:
			r = f3d::utils::fpAcquireNextImageKHR(vk_device, vk_swapchain, UINT64_MAX, vk_presentation_semaphore, nullFence, &vk_present_frame);
			F3D_ASSERT_VK(r, VK_SUCCESS, "Acquire swapchain next image");

			bool il = _device->initImageLayout(vk_images[vk_present_frame], VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, 0, VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT, VK_IMAGE_ASPECT_COLOR_BIT);
			F3D_ASSERT(il, "Init back image buffer layout");

			vkDeviceWaitIdle(_device->vk_device);
		}


		GLFWwindow*		WindowImpl::getGLFWwindow() {
			return		_window;
		}

		void			WindowImpl::initSurface() {
			if (vk_surface == 0) {
				VkResult res = glfwCreateWindowSurface(vk_instance, _window, 0, &vk_surface);
				F3D_ASSERT_VK(res, VK_SUCCESS, "Surface KHR creation");
			}
			F3D_ASSERT(_device->getQueueFamilyIndex(true, 0, vk_surface) != UINT32_MAX, "Did not find a queue supporting presentation");
		}

		void			WindowImpl::initFormatAndColor() {
			VkResult					res;
			uint32_t					formatCount = 0;
			VkSurfaceFormatKHR			*surfFormats = 0;

			res = f3d::utils::fpGetPhysicalDeviceSurfaceFormatsKHR(vk_physical_device, vk_surface, &formatCount, NULL);
			F3D_ASSERT_VK(res, VK_SUCCESS, "Get surface format count");

			surfFormats = new VkSurfaceFormatKHR[formatCount];
			res = f3d::utils::fpGetPhysicalDeviceSurfaceFormatsKHR(vk_physical_device, vk_surface, &formatCount, surfFormats);
			F3D_ASSERT_VK(res, VK_SUCCESS, "Get surface formats failed");

			// If the format list includes just one entry of VK_FORMAT_UNDEFINED,
			// the surface has no preferred format.  Otherwise, at least one
			// supported format will be returned.
			if (formatCount == 1 && surfFormats[0].format == VK_FORMAT_UNDEFINED) {
				vk_format = VK_FORMAT_B8G8R8A8_UNORM;
			}
			else {
				vk_format = surfFormats[0].format;
			}
			vk_color_space = surfFormats[0].colorSpace;
		}

		void							WindowImpl::initSwapchain() {
			VkResult					r;
			VkSwapchainKHR				old_swap_chain = vk_swapchain;
			VkSurfaceCapabilitiesKHR	surf_cap;
			VkSwapchainCreateInfoKHR	swap_info;
			uint32_t					presentModeCount = 0;
			VkExtent2D					swapchainExtent;

			std::memset(&swap_info, 0, sizeof(VkSwapchainCreateInfoKHR));
			swap_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;

			// Check the surface capabilities and formats
			r = f3d::utils::fpGetPhysicalDeviceSurfaceCapabilitiesKHR(vk_physical_device, vk_surface, &surf_cap);
			F3D_ASSERT_VK(r, VK_SUCCESS, "Get surface capabilities failed");
			r = f3d::utils::fpGetPhysicalDeviceSurfacePresentModesKHR(vk_physical_device, vk_surface, &presentModeCount, NULL);
			F3D_ASSERT_VK(r, VK_SUCCESS, "get surface present modes count failed");
			VkPresentModeKHR *presentModes = new VkPresentModeKHR[presentModeCount];
			r = f3d::utils::fpGetPhysicalDeviceSurfacePresentModesKHR(vk_physical_device, vk_surface, &presentModeCount, presentModes);
			F3D_ASSERT_VK(r, VK_SUCCESS, "get surface present modes failed");

			if (surf_cap.currentExtent.width == (uint32_t)-1) { // If the surface size is undefined, set to the size of the window.
				swapchainExtent.width = _settings->windowWidth;
				swapchainExtent.height = _settings->windowHeight;
			}
			else { 	// If the surface size is defined, the swap chain size must match
				swapchainExtent = surf_cap.currentExtent;
				_settings->windowWidth = surf_cap.currentExtent.width;
				_settings->windowHeight = surf_cap.currentExtent.height;
			}

			
			//Compute optimal present mode
			VkPresentModeKHR	swapchainPresentMode = VK_PRESENT_MODE_FIFO_KHR;
			for (size_t i = 0; i < presentModeCount; i++) {
				if (presentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR) {
					swapchainPresentMode = VK_PRESENT_MODE_MAILBOX_KHR;
					break;
				}
				if ((swapchainPresentMode != VK_PRESENT_MODE_MAILBOX_KHR) &&
					(presentModes[i] == VK_PRESENT_MODE_IMMEDIATE_KHR)) {
					swapchainPresentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
				}
			}

			uint32_t desiredNumberOfSwapchainImages = 2;
			if ((surf_cap.maxImageCount > 0) && (desiredNumberOfSwapchainImages > surf_cap.maxImageCount)) {
				desiredNumberOfSwapchainImages = surf_cap.maxImageCount;
			}

			VkSurfaceTransformFlagsKHR preTransform;
			if (surf_cap.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR) {
				preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
			}
			else {
				preTransform = surf_cap.currentTransform;
			}

			swap_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
			swap_info.pNext = NULL;
			swap_info.surface = vk_surface;
			swap_info.minImageCount = desiredNumberOfSwapchainImages;
			swap_info.imageFormat = vk_format; //Default is VK_FORMAT_B8G8R8A8_UNORM; 
			swap_info.imageColorSpace = vk_color_space; //No choice, only one value is available VK_COLORSPACE_SRGB_NONLINEAR_KHR;
			swap_info.imageExtent.width = swapchainExtent.width;
			swap_info.imageExtent.height = swapchainExtent.height;
			swap_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
			swap_info.preTransform = (VkSurfaceTransformFlagBitsKHR)preTransform;
			swap_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
			swap_info.imageArrayLayers = 1;
			swap_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			swap_info.queueFamilyIndexCount = 0;
			swap_info.pQueueFamilyIndices = NULL;
			swap_info.presentMode = swapchainPresentMode;
			swap_info.oldSwapchain = old_swap_chain;
			swap_info.clipped = true;

			r = f3d::utils::fpCreateSwapchainKHR(vk_device, &swap_info, NULL, &vk_swapchain);
			F3D_ASSERT_VK(r, VK_SUCCESS, "Create swap chain failed");
		}

		void				WindowImpl::initImages() {
			VkResult		r;

			if (vk_images != nullptr)
				delete [] vk_images;

			r = f3d::utils::fpGetSwapchainImagesKHR(vk_device, vk_swapchain, &vk_image_count, NULL);
			F3D_ASSERT_VK(r, VK_SUCCESS, "Get swap chain image count failed");

			vk_images = new VkImage[vk_image_count];

			r = f3d::utils::fpGetSwapchainImagesKHR(vk_device, vk_swapchain, &vk_image_count, vk_images);
			F3D_ASSERT_VK(r, VK_SUCCESS, "Get swap chain Images failed");

			for (uint32_t i = 0; i < vk_image_count; i++) {
				_device->initImageLayout(vk_images[i], VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, 0, VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT, VK_IMAGE_ASPECT_COLOR_BIT);
			}
		}

	}
}

