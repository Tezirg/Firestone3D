#pragma once

#ifndef _F3D_OBJECT_H
#define _F3D_OBJECT_H

#include <iostream>
#include <memory>
#include <cstdint>

#include "core/plateform.h"
#include "tree/node.h"

namespace f3d {
	namespace tree {
		class F3D_API Object {
		public:
			Object();
			~Object();

			Node*			getRoot();
			void			setRoot(Node *);

			void			translate(const glm::vec3& translation);
			void			scale(const glm::vec3& scaling);
			void			rotate(const float degrees, const glm::vec3& axis);

		protected:
			f3d::tree::Node*					_root;
			//std::list<f3d::tree::Material>	_mats;
			//std::list<f3d::tree::Animation>	_anims;
			//std::list<f3d::tree::Texture>		_texts;
		};

	}
}

#endif