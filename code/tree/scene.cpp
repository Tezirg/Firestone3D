#include "tree/scene.h"
#include "tree/scene_impl.h"
#include "tree/camera_impl.h"

namespace f3d {
	namespace tree {
		Scene::Scene() : _dirty(true), _camera(new f3d::tree::CameraImpl()){

		}

		Scene::~Scene() {
		}

		std::weak_ptr<f3d::tree::Camera>				Scene::getCamera() {
			return _camera;
		}

		std::list< f3d::tree::Object * >&				Scene::getObjects() {
			return _objects;
		}

		std::list< f3d::tree::Light * >&				Scene::getLights() {
			return _lights;
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

		Scene*		Scene::loadFromFile(const std::string& path) {
			return SceneImpl::loadFromFile(path);
		}

	}
}