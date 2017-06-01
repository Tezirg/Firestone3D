#include "material_impl.h"

namespace f3d {
	namespace tree {
		MaterialImpl::MaterialImpl(const std::string& name, std::shared_ptr< f3d::core::PhysicalDevice >& phys, std::shared_ptr< f3d::core::Device >& device) :
			Material::Material(name), DescriptorContainer::DescriptorContainer(phys, device), AttributeContainer::AttributeContainer(phys, device),
			_physical(phys), _device(device)
		{

			createAttribute();

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

			updateAttribute();
			updateDescriptorSet();
		}

		MaterialImpl::~MaterialImpl() {
			//vkFreeDescriptorSets(_device->vk_device, _desc_pool, 1, &_descriptor);
			//vkDestroyDescriptorPool(_device->vk_device, _desc_pool, nullptr);
			//vkDestroyDescriptorSetLayout(_device->vk_device, _desc_layout, nullptr);
			vkDestroyBuffer(_device->vk_device, _buffer, nullptr);
			vkFreeMemory(_device->vk_device, _memory, nullptr);
			std::cout << "Destructor: " << __FILE__ << std::endl;
		}

		void						MaterialImpl::createAttribute() {
			VkResult				r;
			VkBufferCreateInfo		buff_info;
			VkMemoryRequirements	mem_reqs;
			VkMemoryAllocateInfo	mem_info;

			std::memset(&buff_info, 0, sizeof(VkBufferCreateInfo));
			buff_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			buff_info.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
			buff_info.size = 6 * 4 * sizeof(float) + sizeof(float);
			r = vkCreateBuffer(_device->vk_device, &buff_info, NULL, &_buffer);
			F3D_ASSERT_VK(r, VK_SUCCESS, "Creation vk buffer failed");

			vkGetBufferMemoryRequirements(_device->vk_device, _buffer, &mem_reqs);
			std::memset(&mem_info, 0, sizeof(VkMemoryAllocateInfo));
			mem_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
			mem_info.memoryTypeIndex = _physical->getMemoryIndex(mem_reqs.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
			mem_info.allocationSize = mem_reqs.size;
			r = vkAllocateMemory(_device->vk_device, &mem_info, NULL, &_memory);
			F3D_ASSERT_VK(r, VK_SUCCESS, "Buffer mem allocation failed");

			r = vkBindBufferMemory(_device->vk_device, _buffer, _memory, 0);
			F3D_ASSERT_VK(r, VK_SUCCESS, "Bind memory to buffer fail");
		}

		void					MaterialImpl::updateAttribute() {
			VkResult			r;
			char				*pData;
			glm::vec3			color;

			r = vkMapMemory(_device->vk_device, _memory, 0, VK_WHOLE_SIZE, 0, (void **)&pData);
			F3D_ASSERT_VK(r, VK_SUCCESS, "Can't map material buffer memory");

			std::memcpy(pData, glm::value_ptr(getColor(F3D_COLOR_AMBIENT)), 3 * sizeof(float)); //vec4 ambient_color
			std::memcpy(& pData[4 * sizeof(float)], glm::value_ptr(getColor(F3D_COLOR_DIFFUSE)), 3 * sizeof(float)); //vec4 diffuse_color
			std::memcpy(& pData[2 * 4 * sizeof(float)], glm::value_ptr(getColor(F3D_COLOR_SPECULAR)), 3 * sizeof(float)); //vec4 specular_color
			std::memcpy(& pData[3 * 4 * sizeof(float)], glm::value_ptr(getColor(F3D_COLOR_EMISSIVE)), 3 * sizeof(float)); //vec4 emissive_color
			std::memcpy(& pData[4 * 4 * sizeof(float)], glm::value_ptr(getColor(F3D_COLOR_REFLECTIVE)), 3 * sizeof(float)); //vec4 reflective_color
			std::memcpy(& pData[5 * 4 * sizeof(float)], &_shininess, sizeof(float)); //float shininess
			vkUnmapMemory(_device->vk_device, _memory);
		}

		VkDescriptorSet							MaterialImpl::getDescriptorSet() {
			return _descriptor;
		}

		void									MaterialImpl::updateDescriptorSet() {
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
			buffer_info.buffer = _buffer;
			vkUpdateDescriptorSets(_device->vk_device, 1, &pWrites, 0, nullptr);

			for (auto it = _textures.begin(); it != _textures.end(); ++it) {
				f3d::tree::TextureImpl * t = dynamic_cast<f3d::tree::TextureImpl *>(it->second);
				t->updateDescriptorSet();
			}
		}

	}
}