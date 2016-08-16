#pragma once

#ifndef _F3D_RENDERER_H
#define _F3D_RENDERER_H

#include <iostream>
#include <memory>

#include "f3d.h"

namespace f3d {
	namespace core {
		class Renderer {
		public:
			Renderer();
			virtual ~Renderer();

			virtual void	render(std::shared_ptr<f3d::tree::Scene> scene) = 0;
			virtual void	display() = 0;

		public:
			std::shared_ptr<f3d::core::Settings>	settings;
		};
	}
}

#endif