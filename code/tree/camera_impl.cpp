#include "camera_impl.h"

namespace f3d {
	namespace tree {
		CameraImpl::CameraImpl(std::shared_ptr< f3d::core::PhysicalDevice >& phys, std::shared_ptr< f3d::core::Device >& device) : 
			DescriptorContainer::DescriptorContainer(phys, device),
			AttributeContainer::AttributeContainer(phys, device),
			_physical(phys), _device(device)
		{
			applyPreset(F3D_CAMERA_PRESET_DEFAULT);
			setName(std::string("DefaultCamera"));
			
			AttributeContainer::addAttribute(0, 32 * sizeof(float), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
			DescriptorContainer::addDescriptor(0);
			DescriptorContainer::addDescriptorBinding(0, 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT);
			
			writeAttribute();
			writeDescriptorSet();
		}

		CameraImpl::~CameraImpl() {
			std::cout << "Destructor: " << __FILE__ << std::endl;
		}

		void								CameraImpl::writeAttribute() {
			AttributeContainer::updateAttribute(0, glm::value_ptr(Camera::_buffer._view), offsetof(t_camera, _view), 16 * sizeof(float));
			AttributeContainer::updateAttribute(0, glm::value_ptr(Camera::_buffer._perspective), offsetof(t_camera, _perspective), 16 * sizeof(float));
		}

		VkDescriptorSet						CameraImpl::getDescriptorSet() {
			return DescriptorContainer::getDescriptorSet(0);
		}

		void								CameraImpl::writeDescriptorSet() {
			VkWriteDescriptorSet			pWrites;
			VkDescriptorBufferInfo			buffer_info;

			//Bind camera buffer with this camera descriptor set
			std::memset(&pWrites, 0, sizeof(VkWriteDescriptorSet));
			pWrites.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			pWrites.dstSet = DescriptorContainer::getDescriptorSet(0);
			pWrites.descriptorCount = 1;
			pWrites.descriptorType = VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			pWrites.pBufferInfo = &buffer_info;
			buffer_info.offset = 0;
			buffer_info.range = VK_WHOLE_SIZE;
			buffer_info.buffer = AttributeContainer::getAttributeBuffer(0);
			vkUpdateDescriptorSets(_device->vk_device, 1, &pWrites, 0, nullptr);
		}

	}
}