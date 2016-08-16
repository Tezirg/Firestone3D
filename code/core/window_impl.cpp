#include "window_impl.h"

namespace f3d {
	namespace core {
		
		WindowImpl::WindowImpl(VkInstance instance, VkPhysicalDevice physical, 
			std::shared_ptr<f3d::core::Device>& device, std::shared_ptr<f3d::core::Settings>& settingsPtr) 
			: vk_instance(instance), vk_physical_device(physical), vk_device(device->vk_device),
			  vk_surface((VkSurfaceKHR)0), vk_swapchain((VkSwapchainKHR)0),
			  _settings(settingsPtr), _device(device), _window(nullptr)
		{
			_monitor = glfwGetPrimaryMonitor();
			_videoMode = glfwGetVideoMode(_monitor);
			glfwWindowHint(GLFW_REFRESH_RATE, _videoMode->refreshRate);
			glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

			vk_present_frame = 0;
			vk_images = nullptr;
			vk_image_count = 0;

			_window = glfwCreateWindow(_settings->windowWidth, _settings->windowHeight, _settings->applicationName.c_str(), NULL, NULL);
			applySettings();
		}

		WindowImpl::~WindowImpl() {
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

			//Destroy old surface if exists
			if (vk_surface != 0)
				vkDestroySurfaceKHR(vk_instance, vk_surface, NULL);
			vk_surface = (VkSurfaceKHR)0;
			initSurface();
			initFormatAndColor();
			initSwapchain();
			initImages();
		}

		void			WindowImpl::swapBuffers() {
			VkResult					r;
			VkFence						presentFence;
			VkFenceCreateInfo			presentFenceInfo;
			VkFence						nullFence = VK_NULL_HANDLE;

			std::memset(&presentFenceInfo, 0, sizeof(VkFenceCreateInfo));
			presentFenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;

			r = vkDeviceWaitIdle(vk_device);
			F3D_ASSERT_VK(r, VK_SUCCESS, "Wait device IDLE");

			r = vkCreateFence(vk_device, &presentFenceInfo, NULL, &presentFence);
			F3D_ASSERT_VK(r, VK_SUCCESS, "Can't create prensentation fence");

			// Get the index of the next available swapchain image:
			r = f3d::utils::fpAcquireNextImageKHR(vk_device, vk_swapchain, UINT64_MAX, VK_NULL_HANDLE, presentFence, &vk_present_frame);
			F3D_ASSERT_VK(r, VK_SUCCESS, "Acquire swapchain next image");

			r = vkWaitForFences(vk_device, 1, &presentFence, VK_TRUE, UINT64_MAX);
			F3D_ASSERT_VK(r, VK_SUCCESS, "Wait for presentation fence");

			vkDestroyFence(vk_device, presentFence, NULL);
		}


		GLFWwindow*		WindowImpl::getGLFWwindow() {
			return		_window;
		}

		void			WindowImpl::initSurface() {
			VkResult res = glfwCreateWindowSurface(vk_instance, _window, 0, &vk_surface);
			F3D_ASSERT_VK(res, VK_SUCCESS, "Surface KHR creation");
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
			F3D_ASSERT_VK(r, VK_SUCCESS, "Get swap chain imge count failed");

			vk_images = new VkImage[vk_image_count];

			r = f3d::utils::fpGetSwapchainImagesKHR(vk_device, vk_swapchain, &vk_image_count, vk_images);
			F3D_ASSERT_VK(r, VK_SUCCESS, "Get swap chain Images failed");

			for (uint32_t i = 0; i < vk_image_count; i++) {
				_device->initImageLayout(vk_images[i], VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, 0, VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT, VK_IMAGE_ASPECT_COLOR_BIT);
			}
		}

	}
}

