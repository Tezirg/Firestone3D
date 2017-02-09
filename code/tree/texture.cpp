#include "tree/texture.h"

namespace f3d {
	namespace tree {
		Texture::Texture(uint32_t width, uint32_t height, uint32_t mip_levels, eTextureType type, eTextureAddressMode mode) :
			_width(width), _height(height), _mip_levels(mip_levels), _type(type), _address_mode(mode) {
		}

		Texture::~Texture() {
		}

		uint32_t							Texture::getWidth() const {
			return _width;
		}

		void								Texture::setWidth(const uint32_t value) {
			_width = value;
		}

		uint32_t							Texture::getHeight() const {
			return _height;
		}

		void								Texture::setHeight(const uint32_t value) {
			_height = value;
		}

		uint32_t							Texture::getMipmapLevel() const {
			return _mip_levels;
		}

		void								Texture::setMipmapLevel(const uint32_t value) {
			_mip_levels = value;
		}

		Texture::eTextureType				Texture::getType() const {
			return _type;
		}

		void								Texture::setType(const eTextureType& value) {
			_type = value;
		}

		Texture::eTextureAddressMode		Texture::getAddressMode() const {
			return _address_mode;
		}

		void								Texture::setAddressMode(const eTextureAddressMode& value) {
			_address_mode = value;
		}

	}// tree::
}// f3d::