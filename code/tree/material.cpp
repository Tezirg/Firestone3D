#include "tree/material.h"

namespace f3d {
	namespace tree {

		Material::Material(const std::string& name) :
			_name(name), _shininess(0.0f) {
		}

		Material::~Material() {
		}

		const std::string&		Material::getName() const {
			return _name;
		}

		void					Material::setname(const std::string& value) {
			_name.assign(value);
		}

		bool					Material::Material::getColor(const eColorType type, glm::vec3& out) const {
			auto it = _colors.find(type);
			if (it == _colors.end())
				return false;
			out.x = it->second.x;
			out.y = it->second.y;
			out.z = it->second.z;
			return true;
		}

		void					Material::setColor(const eColorType type, glm::vec3& in) {
			_colors[type] = in;
		}

		void					Material::addTexture(f3d::tree::Texture *texture) {
			_textures.push_back(texture);
			std::cout << "Texture added: " << texture->getWidth() << ';' << texture->getHeight() << std::endl;
		}
		void					Material::removeTexture(f3d::tree::Texture *texture) {
			_textures.remove(texture);
		}

		const std::list<f3d::tree::Texture *>& Material::getTextures(void) const {
			return _textures;
		}

		float					Material::getShininess(void) const {
			return _shininess;
		}

		void					Material::setShininess(const float value) {
			_shininess = value;
		}

	}// tree::
}// f3d::