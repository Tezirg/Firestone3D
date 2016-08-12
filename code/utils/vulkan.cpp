#include "vulkan.h"

namespace f3d {
	namespace utils {

		bool							queryInstancePFN(VkInstance instance) {
			GET_INSTANCE_PROC_ADDR(instance, GetPhysicalDeviceSurfaceSupportKHR);
			if (f3d::utils::fpGetPhysicalDeviceSurfaceSupportKHR == NULL)
				return false;

			GET_INSTANCE_PROC_ADDR(instance, GetPhysicalDeviceSurfaceCapabilitiesKHR);
			if (f3d::utils::fpGetPhysicalDeviceSurfaceCapabilitiesKHR == NULL)
				return false;

			GET_INSTANCE_PROC_ADDR(instance, GetPhysicalDeviceSurfaceFormatsKHR);
			if (f3d::utils::fpGetPhysicalDeviceSurfaceFormatsKHR == NULL)
				return false;
			
			GET_INSTANCE_PROC_ADDR(instance, GetPhysicalDeviceSurfacePresentModesKHR);
			if (f3d::utils::fpGetPhysicalDeviceSurfacePresentModesKHR == NULL)
				return false;
			
			GET_INSTANCE_PROC_ADDR(instance, GetSwapchainImagesKHR);
			if (f3d::utils::fpGetSwapchainImagesKHR == NULL)
				return false;

			return true;
		}

		bool							queryDevicePFN(VkDevice device) {
			GET_DEVICE_PROC_ADDR(device, CreateSwapchainKHR);
			if (f3d::utils::fpCreateSwapchainKHR == NULL)
				return false;
			
			GET_DEVICE_PROC_ADDR(device, DestroySwapchainKHR);
			if (f3d::utils::fpDestroySwapchainKHR == NULL)
				return false;
			
			GET_DEVICE_PROC_ADDR(device, GetSwapchainImagesKHR);
			if (f3d::utils::fpGetSwapchainImagesKHR == NULL)
				return false;
			
			GET_DEVICE_PROC_ADDR(device, AcquireNextImageKHR);
			if (f3d::utils::fpAcquireNextImageKHR == NULL)
				return false;
			
			GET_DEVICE_PROC_ADDR(device, QueuePresentKHR);
			if (f3d::utils::fpQueuePresentKHR == NULL)
				return false;

			return true;
		}

		char const*						vkResultToString(VkResult code) {
			static const char			*positive_code_strs[] = {
				"VK_SUCCESS",
				"VK_NOT_READY",
				"VK_TIMEOUT",
				"VK_EVENT_SET",
				"VK_EVENT_RESET",
				"VK_INCOMPLETE",
			};
			static const char			*negative_codes_strs[] = {
				"VK_NEGATIVE_CODE_UNKNOWN",
				"VK_ERROR_OUT_OF_HOST_MEMORY",
				"VK_ERROR_OUT_OF_DEVICE_MEMORY",
				"VK_ERROR_INITIALIZATION_FAILED",
				"VK_ERROR_DEVICE_LOST",
				"VK_ERROR_MEMORY_MAP_FAILED",
				"VK_ERROR_LAYER_NOT_PRESENT",
				"VK_ERROR_EXTENSION_NOT_PRESENT",
				"VK_ERROR_FEATURE_NOT_PRESENT",
				"VK_ERROR_INCOMPATIBLE_DRIVER",
				"VK_ERROR_TOO_MANY_OBJECTS",
				"VK_ERROR_FORMAT_NOT_SUPPORTED"
			};

			int32_t						int_code = static_cast<int32_t>(code);

			if (int_code >= 0)
				return positive_code_strs[int_code];
			return negative_codes_strs[-int_code];
		}
	}
}