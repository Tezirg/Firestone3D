#pragma once

#ifndef _F3D_TYPES_H
#define _F3D_TYPES_H

struct				s_light {
	int				type;
	float			position[3];
	float			direction[3];
	float			innner;
	float			outer;
	float			constant;
	float			linear;
	float			quadratic;
	float			ambient[3];
	float			diffuse[3];
	float			specular[3];
};
typedef struct s_light			f3d_glsl_light;

struct				s_material {
	float			ambient[3];
	float			diffuse[3];
	float			specular[3];
	float			emissive[3];
	unsigned char	enabled_composant[4];
};
typedef struct s_material		f3d_glsl_material;

struct				s_node_uniform {
	float			model[16];
	float			normal[16];
};
typedef struct s_node_uniform	f3d_glsl_node;

struct				s_world_uniform {
	float			view_projection[16];
};
typedef struct s_world_uniform	f3d_glsl_world;


#endif