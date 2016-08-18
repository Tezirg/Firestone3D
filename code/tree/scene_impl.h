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
#include "core/physical_device.h"
#include "core/device.h"

namespace f3d {
	namespace tree {
		class SceneImpl : public Scene {
		public:
			SceneImpl(std::shared_ptr<f3d::core::PhysicalDevice>& physical, std::shared_ptr<f3d::core::Device>& device);
			~SceneImpl();

			void				loadFromFile(const std::string& path);
		private:
			void				recurs_aiNodeToF3d(const aiScene* scene, aiNode *ainode, f3d::tree::Node* f3d_node);
		private:
			std::shared_ptr<f3d::core::PhysicalDevice>	_physical;
			std::shared_ptr<f3d::core::Device>			_device;
		};
	}
}

#endif