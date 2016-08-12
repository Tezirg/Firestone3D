#include "tree/scene.h"
#include "scene_impl.h"

namespace f3d {
	namespace tree {
		Scene::Scene() {
		}

		Scene::~Scene() {
		}

		Scene*		Scene::loadFromFile(const std::string& path) {
			return new SceneImpl();
		}

	}
}