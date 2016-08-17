#pragma once

#ifndef _F3D_OBJECT_H
#define _F3D_OBJECT_H

#include <iostream>
#include <memory>
#include <cstdint>

#include "f3d.h"

namespace f3d {
	namespace tree {
		class Object {
		public:
			Object();
			~Object();

			Node*			getRoot();
			void			setRoot(Node *);

		protected:
			f3d::tree::Node*					_root;
			//std::list<f3d::tree::Material>	_mats;
			//std::list<f3d::tree::Animation>	_anims;
			//std::list<f3d::tree::Texture>		_texts;
		};

	}
}

#endif