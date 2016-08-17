#include "scene_impl.h"

namespace f3d {
	namespace tree {
		SceneImpl::SceneImpl() {

		}

		SceneImpl::~SceneImpl() {

		}

		SceneImpl*			SceneImpl::loadFromFile(const std::string& path) {
			SceneImpl				*s = new SceneImpl();
			Assimp::Importer		importer;
			const aiScene* ai_scene = importer.ReadFile(path.c_str(), aiProcessPreset_TargetRealtime_Fast);
			s->_ai_scene.reset(ai_scene);
			
			if (ai_scene->HasCameras()) {
				s->_camera.reset(new f3d::tree::CameraImpl(ai_scene->mCameras[0]));
			}

			for (uint32_t i = 0; i < ai_scene->mNumLights; i++) {
				s->addLight(new f3d::tree::LightImpl(ai_scene->mLights[i]));
			}
	

			return s;
		}
	}
}