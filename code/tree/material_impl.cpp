#include "material_impl.h"

namespace f3d {
	namespace tree {
		MaterialImpl::MaterialImpl(const std::string& name, std::shared_ptr< f3d::core::PhysicalDevice >& phys, std::shared_ptr< f3d::core::Device >& device) :
			Material::Material(name), DescriptorContainer::DescriptorContainer(phys, device), AttributeContainer::AttributeContainer(phys, device),
			_physical(phys), _device(device)
		{

			AttributeContainer::addAttribute(0, 6 * 4 * sizeof(float) + sizeof(float), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);

			DescriptorContainer::addDescriptor(2);
			DescriptorContainer::addDescriptorBinding(2, 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT);
			_desc_layout = DescriptorContainer::getDescriptorLayout(2);
			_descriptor = DescriptorContainer::getDescriptorSet(2);
		
			//Push default colors values
			_colors[F3D_COLOR_AMBIENT] = glm::vec3(1.0f, 1.0f, 1.0f);
			_colors[F3D_COLOR_DIFFUSE] = glm::vec3(1.0f, 1.0f, 1.0f);
			_colors[F3D_COLOR_SPECULAR] = glm::vec3(1.0f, 1.0f, 1.0f);
			_colors[F3D_COLOR_EMISSIVE] = glm::vec3(1.0f, 1.0f, 1.0f);
			_colors[F3D_COLOR_REFLECTIVE] = glm::vec3(1.0f, 1.0f, 1.0f);

			writeAttribute();
			writeDescriptorSet();
		}

		MaterialImpl::~MaterialImpl() {
			std::cout << "Destructor: " << __FILE__ << std::endl;
		}


		void					MaterialImpl::writeAttribute() {
			char				pData[6 * 4 * sizeof(float) + sizeof(float)];

			std::memcpy(pData, glm::value_ptr(getColor(F3D_COLOR_AMBIENT)), 4 * sizeof(float)); //vec4 ambient_color
			std::memcpy(& pData[4 * sizeof(float)], glm::value_ptr(getColor(F3D_COLOR_DIFFUSE)), 4 * sizeof(float)); //vec4 diffuse_color
			std::memcpy(& pData[2 * 4 * sizeof(float)], glm::value_ptr(getColor(F3D_COLOR_SPECULAR)), 4 * sizeof(float)); //vec4 specular_color
			std::memcpy(& pData[3 * 4 * sizeof(float)], glm::value_ptr(getColor(F3D_COLOR_EMISSIVE)), 4 * sizeof(float)); //vec4 emissive_color
			std::memcpy(& pData[4 * 4 * sizeof(float)], glm::value_ptr(getColor(F3D_COLOR_REFLECTIVE)), 4 * sizeof(float)); //vec4 reflective_color
			std::memcpy(& pData[5 * 4 * sizeof(float)], &_shininess, sizeof(float)); //float shininess

			AttributeContainer::updateAttribute(0, pData, 0, 6 * 4 * sizeof(float) + sizeof(float));
		}

		VkDescriptorSet							MaterialImpl::getDescriptorSet() {
			return _descriptor;
		}

		void									MaterialImpl::writeDescriptorSet() {
			VkWriteDescriptorSet				pWrites;
			VkDescriptorBufferInfo				buffer_info;

			//Bind buffer with this descriptor set
			std::memset(&pWrites, 0, sizeof(VkWriteDescriptorSet));
			pWrites.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			pWrites.dstSet = _descriptor;
			pWrites.descriptorCount = 1;
			pWrites.descriptorType = VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			pWrites.pBufferInfo = &buffer_info;
			buffer_info.offset = 0;
			buffer_info.range = VK_WHOLE_SIZE;
			buffer_info.buffer = AttributeContainer::getAttributeBuffer(0);
			vkUpdateDescriptorSets(_device->vk_device, 1, &pWrites, 0, nullptr);

			for (auto it = _textures.begin(); it != _textures.end(); ++it) {
				f3d::tree::TextureImpl * t = dynamic_cast<f3d::tree::TextureImpl *>(it->second);
				t->updateDescriptorSet();
			}
		}

	}
}