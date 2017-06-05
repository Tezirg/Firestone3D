#pragma once 

#ifndef _F3D_SCENE_H
#define _F3D_SCENE_H

#include <memory>
#include <cstdint>
#include <vector>
#include <list>

#include "core/platform.h"
#include "tree/camera.h"
#include "tree/object.h"
#include "tree/material.h"
#include "tree/node.h"
#include "tree/light.h"
#include "tree/mesh.h"

namespace f3d {
	namespace tree {
		class F3D_API Scene {
		public:
			Scene();
			virtual ~Scene();
		public:
			std::shared_ptr<f3d::tree::Camera>&				getCamera();
			std::list< f3d::tree::Object * >&				getObjects();
			std::list< f3d::tree::Light * >&				getLights();
			std::list< f3d::tree::Material *>&				getMaterials();
			f3d::tree::Material*							getMaterialByName(const std::string& name);

			virtual void									addObject(f3d::tree::Object* mesh);
			void											removeObject(f3d::tree::Object* mesh);

			virtual void									addLight(f3d::tree::Light* light);
			void											removeLight(f3d::tree::Light* light);

			virtual void									addMaterial(f3d::tree::Material* material);
			void											removeMaterial(f3d::tree::Material* material);

			virtual void									loadFromFile(const std::string& path, const std::string& file) = 0;
		protected:
			bool											_dirty;
			std::shared_ptr< f3d::tree::Camera >			_camera;
			std::list< f3d::tree::Object * >				_objects;
			std::list< f3d::tree::Light * >					_lights;
			std::list< f3d::tree::Material * >				_materials;
		};
	}
}

#endif