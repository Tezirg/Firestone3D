#include "light_impl.h"

namespace f3d {
	namespace tree{
		LightImpl::LightImpl(std::shared_ptr< f3d::core::PhysicalDevice >& phys, std::shared_ptr< f3d::core::Device >& device) : 
			_physical(phys), _device(device)
		{
			//Defaults to directional, white color
			setAttenuationConstant(1.0);
			setAttenuationLinear(0.0);
			setAttenuationQuadratic(0.0);
			setColorAmbient(glm::vec3(1.0f));
			setColorDiffuse(glm::vec3(1.0f));
			setColorSpecular(glm::vec3(1.0f));
			setDirection(glm::vec3(0.0f, 1.0f, 0.0f));// From the top
			setName(std::string("DefaultLight"));
			setType(F3D_LIGHT_DIRECTIONAL);
		}
		
		LightImpl::LightImpl(std::shared_ptr< f3d::core::PhysicalDevice >& phys, std::shared_ptr< f3d::core::Device >& device, aiLight *light) : 
			_physical(phys), _device(device)
		{
			setAngleInnerCone(light->mAngleInnerCone);
			setAngleOuterCone(light->mAngleOuterCone);
			setAttenuationConstant(light->mAttenuationConstant);
			setAttenuationLinear(light->mAttenuationLinear);
			setAttenuationQuadratic(light->mAttenuationQuadratic);
			setColorAmbient(glm::vec3(light->mColorAmbient.r, light->mColorAmbient.g, light->mColorAmbient.b));
			setColorDiffuse(glm::vec3(light->mColorDiffuse.r, light->mColorDiffuse.g, light->mColorDiffuse.b));
			setColorSpecular(glm::vec3(light->mColorSpecular.r, light->mColorSpecular.g, light->mColorSpecular.b));
			setDirection(glm::vec3(light->mDirection.x, light->mDirection.y, light->mDirection.z));// From the top
			setName(std::string(light->mName.C_Str()));
			setPosition(glm::vec3(light->mPosition.x, light->mPosition.y, light->mPosition.z));
			setType(F3D_LIGHT_DIRECTIONAL);
		}

		LightImpl::~LightImpl() {
		}

		float					LightImpl::getAngleInnerCone() const	{ return _inner_cone; }
		void					LightImpl::setAngleInnerCone(float val)	{ _inner_cone = val;  }
		float					LightImpl::getAngleOuterCone() const	{ return _outer_cone; }
		void					LightImpl::setAngleOuterCone(float val) { _outer_cone = val; }
		float					LightImpl::getAttenuationConstant() const { return _constant;  }
		void					LightImpl::setAttenuationConstant(float val) { _constant = val; }
		float					LightImpl::getAttenuationLinear() const { return _linear;  }
		void					LightImpl::setAttenuationLinear(float val) { _linear = val; }
		float					LightImpl::getAttenuationQuadratic() const { return _quadratic; }
		void					LightImpl::setAttenuationQuadratic(float val) { _quadratic = val; }
		glm::vec3				LightImpl::getColorAmbient() const { return glm::vec3(_ambient_color); }
		void					LightImpl::setColorAmbient(const glm::vec3& val) { _ambient_color.r = val.x; _ambient_color.g = val.y; _ambient_color.b = val.z; }
		glm::vec3				LightImpl::getColorDiffuse() const { return glm::vec3(_diffuse_color); }
		void					LightImpl::setColorDiffuse(const glm::vec3& val) { _diffuse_color.r = val.x; _diffuse_color.g = val.y; _diffuse_color.b = val.z; }
		glm::vec3				LightImpl::getColorSpecular() const { return glm::vec3(_specular_color); }
		void					LightImpl::setColorSpecular(const glm::vec3& val) { _specular_color.r = val.x; _specular_color.g = val.y; _specular_color.b = val.z; }
		glm::vec3				LightImpl::getDirection() const { return glm::vec3(); }
		void					LightImpl::setDirection(const glm::vec3& val) { 
		}
		std::string				LightImpl::getName() const { return _name; }
		void					LightImpl::setName(const std::string& val) { _name.assign(val); }
		glm::vec3				LightImpl::getPosition() const { return glm::vec3(_position); }
		void					LightImpl::setPosition(const glm::vec3& val) { _position.x = val.x; _position.y = val.y; _position.z = val.z; }

		eLightType				LightImpl::getType() const { return _type; }
		void			 		LightImpl::setType(eLightType val) { _type = val;  }

		void						LightImpl::createAttribute() {
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

		void					LightImpl::updateAttribute() {
			VkResult			r;
			char				*pData;
			glm::vec3			color;

			r = vkMapMemory(_device->vk_device, _memory, 0, VK_WHOLE_SIZE, 0, (void **)&pData);
			F3D_ASSERT_VK(r, VK_SUCCESS, "Can't map material buffer memory");

			std::memcpy(pData, glm::value_ptr(_ambient_color), 3 * sizeof(float)); //vec3 ambient_color
			std::memcpy(&pData[4 * sizeof(float)], glm::value_ptr(_diffuse_color), 3 * sizeof(float)); //vec3 diffuse_color
			std::memcpy(&pData[2 * 4 * sizeof(float)], glm::value_ptr(_specular_color), 3 * sizeof(float)); //vec3 specular_color
			std::memcpy(&pData[3 * 4 * sizeof(float)], & _constant, sizeof(float)); //float constant
			std::memcpy(&pData[3 * 4 * sizeof(float) + sizeof(float)], & _linear, sizeof(float)); //float linear
			std::memcpy(&pData[3 * 4 * sizeof(float) + 2 * sizeof(float)], &_quadratic, sizeof(float)); //float quadratic
			vkUnmapMemory(_device->vk_device, _memory);
		}

		VkDescriptorSet							LightImpl::getDescriptorSet() {
			return _descriptor;
		}

		void									LightImpl::updateDescriptorSet() {
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
		}
	}
}