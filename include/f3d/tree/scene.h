#pragma once 

#ifndef _F3D_SCENE_H
#define _F3D_SCENE_H

#include <memory>
#include <cstdint>
#include <vector>
#include <list>

#include "f3d.h"

namespace f3d {
	namespace tree {
		class Scene {
		public:
			Scene();
			virtual ~Scene();
		public:
			std::weak_ptr<f3d::tree::Camera>				getCamera();
			std::list< f3d::tree::Object * >&				getObjects();
			std::list< f3d::tree::Light * >&				getLights();

			void											addObject(f3d::tree::Object* mesh);
			void											removeObject(f3d::tree::Object* mesh);

			void											addLight(f3d::tree::Light* light);
			void											removeLight(f3d::tree::Light* light);

			static Scene*									loadFromFile(const std::string& path);
		protected:
			bool											_dirty;
			std::shared_ptr< f3d::tree::Camera >			_camera;
			std::list< f3d::tree::Object * >				_objects;
			std::list< f3d::tree::Light * >					_lights;
		};
	}
}

#endif