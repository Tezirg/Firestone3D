#pragma once

#ifndef _F3D_NODE_H
#define _F3D_NODE_H

#include <iostream>
#include <list>
#include <cstdint>
#include <memory>

#include "core/plateform.h"
#include "tree/mesh.h"
#include "utils/transform.h"

namespace f3d {
	namespace tree {
		class F3D_API Node {
		public:
			Node();
			~Node();

			Node*							getParent();
			void							setParent(Node *);

			std::list<Node *>&				getChildren();
			void							addChildren(Node *);
			void							removeChildren(Node *);

			std::list<f3d::tree::Mesh*>&	getMeshes();
			void							addMesh(f3d::tree::Mesh *);
			void							removeMesh(f3d::tree::Mesh *);

			f3d::utils::Transform&			transformation();
		protected:
			Node*						_parent;
			std::list<Node *>			_children;
			std::list<f3d::tree::Mesh*>	_meshes;
			f3d::utils::Transform		_transformation;
		};
	}
}

#endif