#include "light_impl.h"

namespace f3d {
	namespace tree{
		LightImpl::LightImpl(std::shared_ptr< f3d::core::PhysicalDevice >& phys, std::shared_ptr< f3d::core::Device >& device) : 
			_physical(phys), _device(device), _buffer(nullptr), _buffer_size(0)
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

			createAttributeBuffer();
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

			createAttributeBuffer();
		}

		LightImpl::~LightImpl() {
			std::cout << "Destructor: " << __FILE__ << std::endl;
			if (_buffer != nullptr)
				delete[] _buffer;
		}

		void						LightImpl::createAttributeBuffer() {
			_buffer_size = (sizeof(float) * 4) * 5; //vec4 attributes
			_buffer_size += sizeof(float) * 5; //float attributes
			_buffer_size += sizeof(uint32_t); //uint type

			_buffer = new char[_buffer_size];

			F3D_ASSERT(_buffer != nullptr, "Allocating light buffer failed");
		}

		void					LightImpl::updateAttributeBuffer() {
			char				*pData = _buffer;

			std::memcpy(&pData[0], glm::value_ptr(_position), 4 * sizeof(float)); //vec4 position
			std::memcpy(&pData[1 * 4 * sizeof(float)], glm::value_ptr(_direction), 4 * sizeof(float)); //vec4 direction
			std::memcpy(&pData[2 * 4 * sizeof(float)], glm::value_ptr(_ambient_color), 4 * sizeof(float)); //vec4 ambient_color
			std::memcpy(&pData[3 * 4 * sizeof(float)], glm::value_ptr(_diffuse_color), 4 * sizeof(float)); //vec4 diffuse_color
			std::memcpy(&pData[4 * 4 * sizeof(float)], glm::value_ptr(_specular_color), 4 * sizeof(float)); //vec4 specular_color
			
			std::memcpy(&pData[5 * 4 * sizeof(float)], &_inner_cone, sizeof(float)); //float inner_cone
			std::memcpy(&pData[5 * 4 * sizeof(float) + sizeof(float)], &_outer_cone, sizeof(float)); //float outer_cone
			std::memcpy(&pData[5 * 4 * sizeof(float) + 2 * sizeof(float)], &_constant, sizeof(float)); //float constant
			std::memcpy(&pData[5 * 4 * sizeof(float) + 3 * sizeof(float)], &_linear, sizeof(float)); //float linear
			std::memcpy(&pData[5 * 4 * sizeof(float) + 4 * sizeof(float)], &_quadratic, sizeof(float)); //float quadratic

			std::memcpy(&pData[5 * 4 * sizeof(float) + 5 * sizeof(float)], &_type, sizeof(float)); //uint type
		}

		bool								LightImpl::getAttributeBuffer(void **buf, uint32_t& size) {
			if (_buffer == nullptr || _buffer_size == 0)
				return false;
			*buf = _buffer;
			size = _buffer_size;
			return true;
		}
	}
}