#include "tree/scene.h"
#include "tree/scene_impl.h"
#include "tree/camera_impl.h"

namespace f3d {
	namespace tree {
		Scene::Scene() : _dirty(true), _camera(nullptr) {

		}

		Scene::~Scene() {
		}

		std::shared_ptr<f3d::tree::Camera>&				Scene::getCamera() {
			return _camera;
		}

		std::list< f3d::tree::Object * >&				Scene::getObjects() {
			return _objects;
		}

		std::list< f3d::tree::Light * >&				Scene::getLights() {
			return _lights;
		}

		std::list< f3d::tree::Material *>&				Scene::getMaterials() {
			return _materials;
		}

		f3d::tree::Material*							Scene::getMaterialByName(const std::string& name) {
			for (auto it = _materials.begin(); it != _materials.end(); ++it) {
				if ((*it)->getName() == name)
					return *it;
			}
			return  nullptr;
		}

		void											Scene::addObject(f3d::tree::Object* obj) {
			_objects.push_back(obj);
		}

		void											Scene::removeObject(f3d::tree::Object* obj) {
			_objects.remove(obj);
		}
		void											Scene::addLight(f3d::tree::Light* light) {
			_lights.push_back(light);
			_dirty = true;
		}
		void											Scene::removeLight(f3d::tree::Light* light) {
			_lights.remove(light);
			_dirty = true;
		}

		void											Scene::addMaterial(f3d::tree::Material* material) {
			_materials.push_back(material);
			_dirty = true;
		}

		void											Scene::removeMaterial(f3d::tree::Material* material) {
			_materials.remove(material);
			_dirty = true;
		}

	}
}