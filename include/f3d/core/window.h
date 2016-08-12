#pragma once

#ifndef __F3D_WINDOW_H
#define __F3D_WINDOW_H

#include <memory>
#include <cstdint>

#include "core/settings.h"

namespace f3d {
	namespace core {
		class Window {
		protected:
			Window();
		public:
			virtual ~Window();
			virtual void			applySettings(void) = 0;
		};
	}
}

#endif