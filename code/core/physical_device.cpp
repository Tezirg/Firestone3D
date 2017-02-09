#include "physical_device.h"

namespace f3d {
	namespace core {
		PhysicalDevice::PhysicalDevice(VkInstance instance) : _vk_instance(instance) {
			VkResult		r;

			r = vkEnumeratePhysicalDevices(_vk_instance, &_device_count, NULL);
			F3D_ASSERT_VK(r, VK_SUCCESS, "Can't count physical devices")
			F3D_ASSERT(_device_count > 0, "No vulkan compatible device found !")

			_devices = new VkPhysicalDevice[_device_count];
			r = vkEnumeratePhysicalDevices(_vk_instance, &_device_count, _devices);
			F3D_ASSERT_VK(r, VK_SUCCESS, "Enumerate physical devices failed")

			_props = new VkPhysicalDeviceProperties[_device_count];
			_mem_props = new VkPhysicalDeviceMemoryProperties[_device_count];
			_features = new VkPhysicalDeviceFeatures[_device_count];
			for (uint32_t i = 0; i < _device_count; i++) {
				vkGetPhysicalDeviceProperties(_devices[i], &(_props[i]));
				vkGetPhysicalDeviceFeatures(_devices[i], &(_features[i]));
				vkGetPhysicalDeviceMemoryProperties(_devices[i], &_mem_props[i]);
			}

			//Defaults to first device
			vk_physical_device = _devices[0];
			vk_physical_device_props = _props[0];
			vk_physical_device_mem_props = _mem_props[0];
			vk_physical_device_features = _features[0];
		}
		PhysicalDevice::~PhysicalDevice() {}

		int		PhysicalDevice::getMemoryIndex(uint32_t typeBits, uint32_t requirementsBits) {
			for (uint32_t i = 0; i < 32 && i < vk_physical_device_mem_props.memoryTypeCount; i++) {
				if ((typeBits & 1) == 1) {
					// Type is available, does it match requirements ?
					if ((vk_physical_device_mem_props.memoryTypes[i].propertyFlags & requirementsBits) == requirementsBits) {
						return i;
					}
				}
				typeBits >>= 1;
			}
			return -1;
		}

	}
}