#include "tree/material.h"

namespace f3d {
	namespace tree {

		Material::Material(const std::string& name) :
			_name(name), _shininess(0.0f) {
			_color_mask = F3D_COLOR_UNDEFINED;
			_texture_mask = F3D_TEXTURE_UNDEFINED;
		}

		Material::~Material() {
		}

		const std::string&		Material::getName() const {
			return _name;
		}

		void					Material::setname(const std::string& value) {
			_name.assign(value);
		}

		bool					Material::getColor(const eColorType type, glm::vec3& out) const {
			auto it = _colors.find(type);
			if (it == _colors.end())
				return false;
			out.x = it->second.x;
			out.y = it->second.y;
			out.z = it->second.z;
			return true;
		}

		const glm::vec3&		Material::getColor(const eColorType type) const {
			static glm::vec3	_black(0.0f, 0.0f, 0.0f);
			auto it = _colors.find(type);
			if (it == _colors.end())
				return _black;
			return it->second;
		}

		void					Material::setColor(const eColorType type, glm::vec3& in) {
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

		void					Material::addTexture(f3d::tree::Texture *texture) {
			_textures.push_back(texture);
			//Add to bitmask
			_texture_mask |= texture->getType();
		}
		void					Material::removeTexture(f3d::tree::Texture *texture) {
			_textures.remove(texture);
			//Removes from bitmask
			_texture_mask &= ~texture->getType();
		}
		void					Material::removeTexture(const TextureTypeFlags type) {
			std::remove_if(_textures.begin(), _textures.end(), [&type](auto it) {
				return it->getType() == type; //Lambda match all textures == remove type
			});
			//Removes from bitmask
			_texture_mask &= ~type;
		}

		const std::list<f3d::tree::Texture *>& Material::getTextures(void) const {
			return _textures;
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