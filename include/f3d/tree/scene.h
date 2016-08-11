#pragma once 

#ifndef _F3D_SCENE_H
#define _F3D_SCENE_H

#include <memory>
#include <cstdint>
#include <vector>
#include <assimp/scene.h>
#include <assimp/mesh.h>
#include <assimp/light.h>
#include <assimp/camera.h>

#include "tree/mesh.h"
#include "tree/light.h"
#include "tree/camera.h"
#include "utils/logging.h"

namespace f3d {
	namespace tree {
		class Scene {
		public:
			Scene();
			~Scene();
		private:
			std::unique_ptr< aiScene >							_ai_scene;
			std::unique_ptr< f3d::tree::Camera >				_camera;
			std::vector< std::shared_ptr<f3d::tree::Mesh> >		_meshes;
			std::vector< std::shared_ptr<f3d::tree::Light> >	_lights;
		};
	}
}

#endif