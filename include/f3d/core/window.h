#pragma once

#ifndef __F3D_WINDOW_H
#define __F3D_WINDOW_H

#include <memory>
#include <cstdint>

#include "core/platform.h"
#include "core/settings.h"

namespace f3d {
	namespace core {
		class F3D_API Window {
		public:
			Window();
			virtual ~Window();
			virtual void			applySettings(void) = 0;
			virtual void			swapBuffers() = 0;
			int						width() const;
			int						height() const;
			
		protected:
			std::shared_ptr<f3d::core::Settings>		_settings;
		};
	}
}

#endif