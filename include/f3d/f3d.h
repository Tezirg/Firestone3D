#pragma once

#ifndef __F3D_H
#define __F3D_H

#if defined(F3D_EXPORT_DYNAMIC)
#define F3D_API __declspec(dllexport)
#elif defined(F3D_IMPORT_DYNAMIC)
#define F3D_API __declspec(dllimport)
#else
#define F3D_API
#endif

#if (defined(F3D_EXPORT_DYNAMIC) || defined(F3D_EXPORT_STATIC))
#define F3D_EXPORTS
#endif

#include "version.h"
#include "core/firestone.h"
#include "core/settings.h"
#include "core/window.h"
#include "tree/scene.h"
#include "tree/camera.h"
#include "tree/mesh.h"
#include "tree/light.h"
#include "utils/logging.h"


#endif