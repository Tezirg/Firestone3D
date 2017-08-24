#pragma once

#ifndef _F3D_RENDERER_H
#define _F3D_RENDERER_H

#include <iostream>
#include <memory>

#include "core/platform.h"
#include "core/settings.h"
#include "tree/scene.h"

namespace f3d {
	namespace core {
		class F3D_API Renderer {
		public:
			Renderer();
			virtual ~Renderer();

			Renderer(Renderer& copy_oth) = delete; // No copies
			Renderer(Renderer&& move_oth) = delete; // No moves
			Renderer& operator=(Renderer& copy_oth) = delete; // No copy assignement
			Renderer& operator=(Renderer&& move_oth) = delete; // No move assignement


			virtual void	render(std::shared_ptr<f3d::tree::Scene> scene) = 0;
			virtual void	display() = 0;

		public:
			std::shared_ptr<f3d::core::Settings>	settings;
		};
	}
}

#endif