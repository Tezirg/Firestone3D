#pragma once

#ifndef __F3D_H
#define __F3D_H

#include "version.h"
#include "core/platform.h"
#include "core/types.h"

namespace f3d {
	namespace core {
		class F3D_API Firestone;
		class F3D_API Settings;
		class F3D_API Window;
		class F3D_API Renderer;
	}
	namespace tree {
		class F3D_API Scene;
		class F3D_API Camera;
		class F3D_API Light;
		class F3D_API Object;
		class F3D_API Node;
		class F3D_API Mesh;
		class F3D_API Material;
		class F3D_API Texture;
	}
	namespace utils {
		class F3D_API Timer;
		class F3D_API Transform;
		class F3D_API KeyInput;
		class F3D_API MouseInput;
		class F3D_API JoystickInput;
	}
}


#include "core/firestone.h"
#include "core/settings.h"
#include "core/window.h"
#include "core/renderer.h"
#include "tree/scene.h"
#include "tree/object.h"
#include "tree/camera.h"
#include "tree/light.h"
#include "tree/node.h"
#include "tree/mesh.h"
#include "tree/material.h"
#include "tree/texture.h"
#include "utils/timer.h"
#include "utils/transform.h"
#include "utils/inputs.h"
#include "utils/logging.h"

#endif