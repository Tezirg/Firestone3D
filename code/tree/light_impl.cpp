#include "light_impl.h"

namespace f3d {
	namespace tree{
		LightImpl::LightImpl(std::shared_ptr< f3d::core::PhysicalDevice >& phys, std::shared_ptr< f3d::core::Device >& device) : 
			_physical(phys), _device(device), _buffer(nullptr), _buffer_size(0)
		{
			createProperties();
		}
		
		LightImpl::LightImpl(std::shared_ptr< f3d::core::PhysicalDevice >& phys, std::shared_ptr< f3d::core::Device >& device, aiLight *light) : 
			_physical(phys), _device(device), _buffer(nullptr), _buffer_size(0)
		{
			setSpotCutoff(light->mAngleOuterCone);
			setAttenuationConstant(light->mAttenuationConstant);
			setAttenuationLinear(light->mAttenuationLinear);
			setAttenuationQuadratic(light->mAttenuationQuadratic);
			setColorAmbient(glm::vec4(light->mColorAmbient.r, light->mColorAmbient.g, light->mColorAmbient.b, 1.0f));
			setColorDiffuse(glm::vec4(light->mColorDiffuse.r, light->mColorDiffuse.g, light->mColorDiffuse.b, 1.0f));
			setColorSpecular(glm::vec4(light->mColorSpecular.r, light->mColorSpecular.g, light->mColorSpecular.b, 1.0f));
			setDirection(glm::vec3(light->mDirection.x, light->mDirection.y, light->mDirection.z));
			setName(std::string(light->mName.C_Str()));
			setPosition(glm::vec3(light->mPosition.x, light->mPosition.y, light->mPosition.z));
			ASSIMP_LIGHT_2_F3D(light->mType, _type);

			createProperties();
		}

		LightImpl::LightImpl(std::shared_ptr< f3d::core::PhysicalDevice >& phys, std::shared_ptr< f3d::core::Device >& device, const f3d::tree::Light& oth) :
			_physical(phys), _device(device), _buffer(nullptr), _buffer_size(0)
		{
			setName(oth.getName());
			setType(oth.getType());
			setSpotExponent(oth.getSpotExponent());
			setSpotCutoff(oth.getSpotCutoff());
			setAttenuationConstant(oth.getAttenuationConstant());
			setAttenuationLinear(oth.getAttenuationLinear());
			setAttenuationQuadratic(oth.getAttenuationQuadratic());
			setColorAmbient(oth.getColorAmbient());
			setColorDiffuse(oth.getColorDiffuse());
			setColorSpecular(oth.getColorSpecular());
			setDirection(oth.getDirection());
			setPosition(oth.getPosition());

			createProperties();
		}


		LightImpl::~LightImpl() {
			if (_buffer != nullptr)
				delete[] _buffer;
			std::cout << "Destructor: " << __FILE__ << std::endl;
		}

		uint32_t					LightImpl::bufferSize() {
			uint32_t size = sizeof(float) * 4 * 5; // contains five vec4
			size += sizeof(float) * 6; // And 6 floats
			size += sizeof(uint32_t) * 2; // And two integers
			// Adjust buffer size to align on 4N
			// For use with layout std140 mostly
			size += size % (sizeof(float) * 4);

			return size;
		}

		void						LightImpl::createProperties() {
			_buffer_size = bufferSize();
			
			// Adjust buffer size to align on 4N
			// _buffer_size += _buffer_size % (sizeof(float) * 4);
			_buffer = new char[_buffer_size];
			F3D_ASSERT(_buffer != nullptr, "Allocating light buffer failed");
			std::memset(_buffer, 0, _buffer_size);
		}

		void					LightImpl::updateProperties() {
			char				*pData = _buffer;

			std::memcpy(&pData[0], glm::value_ptr(_position), 4 * sizeof(float)); //vec4 position
			std::memcpy(&pData[1 * 4 * sizeof(float)], glm::value_ptr(_direction), 4 * sizeof(float)); //vec4 direction
			std::memcpy(&pData[2 * 4 * sizeof(float)], glm::value_ptr(_ambient_color), 4 * sizeof(float)); //vec4 ambient_color
			std::memcpy(&pData[3 * 4 * sizeof(float)], glm::value_ptr(_diffuse_color), 4 * sizeof(float)); //vec4 diffuse_color
			std::memcpy(&pData[4 * 4 * sizeof(float)], glm::value_ptr(_specular_color), 4 * sizeof(float)); //vec4 specular_color
			
			std::memcpy(&pData[5 * 4 * sizeof(float)], &_spot_exponent, sizeof(float)); //float spot_exponent
			std::memcpy(&pData[5 * 4 * sizeof(float) + sizeof(float)], &_spot_cutoff, sizeof(float)); //float spot_cutoff
			std::memcpy(&pData[5 * 4 * sizeof(float) + 2 * sizeof(float)], &_spot_cos_cutoff, sizeof(float)); //float spot_cos_cutoff


			std::memcpy(&pData[5 * 4 * sizeof(float) + 3 * sizeof(float)], &_constant, sizeof(float)); //float constant
			std::memcpy(&pData[5 * 4 * sizeof(float) + 4 * sizeof(float)], &_linear, sizeof(float)); //float linear
			std::memcpy(&pData[5 * 4 * sizeof(float) + 5 * sizeof(float)], &_quadratic, sizeof(float)); //float quadratic

			std::memcpy(&pData[5 * 4 * sizeof(float) + 6 * sizeof(float)], &_type, sizeof(uint32_t)); //uint type
		}

		bool								LightImpl::getProperties(void **buf, uint32_t& size) {
			if (_buffer == nullptr || _buffer_size == 0)
				return false;
			updateProperties();
			*buf = _buffer;
			size = _buffer_size;
			return true;
		}
	}
}