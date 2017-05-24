#pragma once

#ifndef __F3D_TYPES_H
#define __F3D_TYPES_H

#include <cstdint>

namespace f3d {
	enum eColorType {
		F3D_COLOR_UNDEFINED = 0,
		F3D_COLOR_AMBIENT = 1,
		F3D_COLOR_DIFFUSE = 2,
		F3D_COLOR_SPECULAR = 3,
		F3D_COLOR_EMMISIVE = 4,
		F3D_COLOR_REFLECTIVE = 5,
		F3D_COLOR_BEGIN_RANGE = F3D_COLOR_UNDEFINED,
		F3D_COLOR_END_RANGE = F3D_COLOR_REFLECTIVE,
		F3D_COLOR_RANGE_SIZE = (F3D_COLOR_END_RANGE - F3D_COLOR_BEGIN_RANGE + 1),
		F3D_COLOR_MAX_ENUM = 0x7FFFFFFF
	};
} // f3d::

#endif