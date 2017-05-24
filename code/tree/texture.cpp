#include "tree/texture.h"

namespace f3d {
	namespace tree {
		Texture::Texture(uint32_t width, uint32_t height, uint32_t mip_levels, eTextureTypeBits type, eTextureAddressMode mode[3]) :
			_width(width), _height(height), _mip_levels(mip_levels), _type(type), _address_mode() {
			_address_mode[0] = mode[0];
			_address_mode[1] = mode[1];
			_address_mode[2] = mode[2];
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

		f3d::eTextureType					Texture::getType() const {
			return _type;
		}

		void								Texture::setType(const eTextureType& value) {
			_type = value;
		}

		f3d::eTextureAddressMode			Texture::getAddressMode() const {
			return _address_mode[0];
		}

		void								Texture::setAddressMode(const eTextureAddressMode& value) {
			_address_mode[0] = value;
		}

	}// tree::
}// f3d::