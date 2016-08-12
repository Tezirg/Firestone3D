#pragma once 

#ifndef _F3D_SCENE_H
#define _F3D_SCENE_H

#include <memory>
#include <cstdint>

#include "f3d.h"

namespace f3d {
	namespace tree {
		class Scene {
		public:
			Scene();
			virtual ~Scene();
		public:
			static Scene*		loadFromFile(const std::string& path);
		};
	}
}

#endif