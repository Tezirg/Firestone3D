#pragma once

#ifndef _ASSIMP_2_F3D_H
#define _ASSIMP_2_F3D_H

#include <assimp/light.h>
#include <assimp/material.h>

#include "core/types.h"

#define ASSIMP_LIGHT_2_F3D(assimp_type, f3d_target)													\
{																									\
	switch (assimp_type) {																			\
		case aiLightSource_DIRECTIONAL:																\
			f3d_target = f3d::F3D_LIGHT_DIRECTIONAL;												\
			break;																					\
		case aiLightSource_POINT:																	\
			f3d_target = f3d::F3D_LIGHT_POINT;														\
			break;																					\
		case aiLightSource_SPOT:																	\
			f3d_target = f3d::F3D_LIGHT_SPOT;														\
			break;																					\
		case aiLightSource_AREA:																	\
			f3d_target = f3d::F3D_LIGHT_UNDEFINED;													\
			break;																					\
		default:																					\
			f3d_target = f3d::F3D_LIGHT_UNDEFINED;													\
	}																								\
}

#define ASSIMP_TEXTURE_2_F3D(assimp_type, f3d_target)												\
{																									\
	switch (assimp_type) {																			\
		case aiTextureType_AMBIENT:																	\
			f3d_target = f3d::F3D_TEXTURE_AMBIENT;													\
			break;																					\
		case aiTextureType_DIFFUSE:																	\
			f3d_target = f3d::F3D_TEXTURE_DIFFUSE;													\
			break;																					\
		case aiTextureType_SPECULAR:																\
			f3d_target = f3d::F3D_TEXTURE_SPECULAR;													\
			break;																					\
		case aiTextureType_EMISSIVE:																\
			f3d_target = f3d::F3D_TEXTURE_EMISSIVE;													\
			break;																					\
		case aiTextureType_HEIGHT:																	\
			f3d_target = f3d::F3D_TEXTURE_HEIGHT;													\
			break;																					\
		case aiTextureType_NORMALS:																	\
			f3d_target = f3d::F3D_TEXTURE_NORMALS;													\
			break;																					\
		case aiTextureType_SHININESS:																\
			f3d_target = f3d::F3D_TEXTURE_SHININESS;												\
			break;																					\
		case aiTextureType_OPACITY:																	\
			f3d_target = f3d::F3D_TEXTURE_OPACITY;													\
			break;																					\
		case aiTextureType_DISPLACEMENT:															\
			f3d_target = f3d::F3D_TEXTURE_DISPLACEMENT;												\
			break;																					\
		case aiTextureType_LIGHTMAP:																\
			f3d_target = f3d::F3D_TEXTURE_LIGHTMAP;													\
			break;																					\
		case aiTextureType_REFLECTION:																\
			f3d_target = f3d::F3D_TEXTURE_REFLECTION;												\
			break;																					\
		default:																					\
			f3d_target = f3d::F3D_TEXTURE_UNDEFINED;												\
	}																								\
}

#define ASSIMP_ADDRESS_MODE_2_F3D(assimp_type, f3d_target)											\
{																									\
	switch (assimp_type)																			\
	{																								\
		case aiTextureMapMode_Wrap:																	\
			f3d_target = f3d::F3D_ADDRESS_REPEAT;													\
			break;																					\
		case aiTextureMapMode_Clamp:																\
			f3d_target =  f3d::F3D_ADDRESS_CLAMP_EDGE;												\
			break;																					\
		case aiTextureMapMode_Decal:																\
			f3d_target =  f3d::F3D_ADDRESS_CLAMP_BORDER;											\
			break;																					\
		case aiTextureMapMode_Mirror:																\
			f3d_target = f3d::F3D_ADDRESS_MIRROR_REPEAT;											\
			break;																					\
		default:																					\
			f3d_target = f3d::F3D_ADDRESS_UNDEFINED;												\
	}																								\
}

#define ASSIMP_SHADING_MODE_2_F3D(assimp_type, f3d_target)											\
{																									\
	switch (assimp_type)																			\
	{																								\
		case aiShadingMode_Flat:																	\
			f3d_target = f3d::F3D_SHADING_DIFFUSE_FLAT;												\
			break;																					\
		case aiShadingMode_Gouraud:																	\
			f3d_target = f3d::F3D_SHADING_DIFFUSE_LAMBERT;											\
			break;																					\
		case aiShadingMode_Phong:																	\
			f3d_target = f3d::F3D_SHADING_SPECULAR_PHONG;											\
			break;																					\
		case aiShadingMode_Blinn:																	\
			f3d_target = f3d::F3D_SHADING_SPECULAR_BLINN_PHONG;										\
			break;																					\
		case aiShadingMode_Toon:																	\
			f3d_target = f3d::F3D_SHADING_DIFFUSE_TOON;												\
			break;																					\
		case aiShadingMode_OrenNayar:																\
			f3d_target = f3d::F3D_SHADING_DIFFUSE_ORENNAYAR;										\
			break;																					\
		case aiShadingMode_Minnaert:																\
			f3d_target = f3d::F3D_SHADING_DIFFUSE_LAMBERT;											\
			break;																					\
		case aiShadingMode_CookTorrance:															\
			f3d_target = f3d::F3D_SHADING_SPECULAR_COOK_TORRANCE;									\
			break;																					\
		default:																					\
			f3d_target = f3d::F3D_SHADING_UNDEFINED;												\
	}																								\
}

#endif