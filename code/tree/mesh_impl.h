#pragma once

#ifndef __F3D_MESH_IMPL_H
#define __F3D_MESH_IMPL_H

#include <memory>
#include <cstdint>

#include "f3d.h"

#include <assimp/scene.h>
#include <assimp/mesh.h>


namespace f3d {
	namespace tree {
		class MeshImpl : public Mesh {
		public:
			MeshImpl();
			~MeshImpl();
	
			virtual bool			makeRenderReady();
			virtual bool			isRenderReady();
		};
	}
}

#endif