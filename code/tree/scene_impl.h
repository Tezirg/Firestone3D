#pragma once 

#ifndef _F3D_SCENE_IMPL_H
#define _F3D_SCENE_IMPL_H

#include <memory>
#include <cstdint>
#include <vector>
#include <assimp/scene.h>
#include <assimp/mesh.h>
#include <assimp/light.h>
#include <assimp/camera.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include "f3d.h"
#include "tree/camera_impl.h"
#include "tree/light_impl.h"
#include "tree/mesh_impl.h"

namespace f3d {
	namespace tree {
		class SceneImpl : public Scene {
		public:
			SceneImpl();
			~SceneImpl();

			static SceneImpl*	loadFromFile(const std::string& path);
		private:
			std::unique_ptr< const aiScene >							_ai_scene;
		};
	}
}

#endif