#pragma once

#ifndef __F3D_PLATEFORM_H
#define __F3D_PLATEFORM_H

#if (defined(F3D_EXPORT_DYNAMIC) || defined(F3D_EXPORT_STATIC))
#define F3D_EXPORTS
#endif

#if defined(F3D_EXPORTS)
/* We are building this library */
#	define F3D_API __declspec(dllexport)
#else
/* We are using this library */
#	define F3D_API __declspec(dllimport)
#endif


#define F3D_PLATFORM_UNKNOWN		0x00000000
#define F3D_PLATFORM_WINDOWS		0x00010000
#define F3D_PLATFORM_LINUX			0x00020000
#define F3D_PLATFORM_APPLE			0x00040000
#define F3D_PLATFORM_ANDROID		0x00080000
#define F3D_PLATFORM_UNIX			0x00100000

#ifdef F3D_FORCE_PLATFORM_UNKNOWN
#	define F3D_PLATFORM F3D_PLATFORM_UNKNOWN
#elif defined(_WIN32)
#	define F3D_PLATFORM F3D_PLATFORM_WINDOWS
#elif defined(__linux)
#	define F3D_PLATFORM F3D_PLATFORM_LINUX
#elif defined(__APPLE__)
#	define F3D_PLATFORM F3D_PLATFORM_APPLE
#elif defined(__ANDROID__)
#	define F3D_PLATFORM F3D_PLATFORM_ANDROID
#elif defined(__unix)
#	define F3D_PLATFORM F3D_PLATFORM_UNIX
#else
#	define F3D_PLATFORM F3D_PLATFORM_UNKNOWN
#endif

#include <cstdint>
#include <memory>
#include <utility>
#include <iostream>

//Linear algebra dependency
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>


//Images dependency
#include <gli/gli.hpp>
#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#endif //__F3D_PLATEFORM_H