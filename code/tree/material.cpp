#include "tree/material.h"

namespace f3d {
	namespace tree {

		Material::Material(const std::string& name) :
			_name(name), _shininess(16.0f) {
			_color_mask = F3D_COLOR_UNDEFINED;
			_shading = F3D_SHADING_FLAT;
			_texture_mask = F3D_TEXTURE_UNDEFINED;
		}

		Material::~Material() {
			for (auto text = _textures.begin(); text != _textures.end(); ++text)
				delete text->second;
			std::cout << "Destructor: " << __FILE__ << std::endl;
		}

		const std::string&		Material::getName() const {
			return _name;
		}

		void					Material::setname(const std::string& value) {
			_name.assign(value);
		}

		bool					Material::getColor(const eColorType type, glm::vec4& out) const {
			auto it = _colors.find(type);
			if (it == _colors.end())
				return false;
			out.x = it->second.x;
			out.y = it->second.y;
			out.z = it->second.z;
			out.w = it->second.w;
			return true;
		}

		const glm::vec4&		Material::getColor(const eColorType type) const {
			static glm::vec4	_black(0.0f);
			auto it = _colors.find(type);
			if (it == _colors.end())
				return _black;
			return it->second;
		}

		void					Material::setColor(const eColorType type, glm::vec4& in) {
			_colors[type] = in;
			_color_mask |= type;
		}

		bool					Material::removeColor(const eColorType type) {
			auto it = _colors.find(type);
			_color_mask &= ~type; // Removes from bitmask
			if (it == _colors.end())
				return false;
			_colors.erase(it);
			return true;
		}

		ColorTypeFlags			Material::colorFlags() const {
			return _color_mask;
		}

		void					Material::setShading(const eShadingType& type) {
			_shading = type;
		}

		eShadingType			Material::shadingFlags(void) const {
			return _shading;
		}

		void					Material::addTexture(f3d::tree::Texture *texture) {
			_textures[texture->getType()] = texture;
			//Add to bitmask
			_texture_mask |= texture->getType();
		}
		void					Material::removeTexture(f3d::tree::Texture *texture) {
			_textures.erase(texture->getType());
			//Removes from bitmask
			_texture_mask &= ~texture->getType();
		}
		void					Material::removeTexture(const TextureTypeFlags type) {
			_textures.erase(type);
			//Removes from bitmask
			_texture_mask &= ~type;
		}

		f3d::tree::Texture*		Material::getTexture(const TextureTypeFlags type) const {
			auto it = _textures.find(type);
			if (it != _textures.end())
				return it->second;
			else
				return nullptr;
		}

		TextureTypeFlags		Material::textureFlags() const {
			return _texture_mask;
		}

		float					Material::getShininess(void) const {
			return _shininess;
		}

		void					Material::setShininess(const float value) {
			_shininess = value;
		}

	}// tree::
}// f3d::