#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
#extension GL_OES_standard_derivatives : enable

#pragma glslify: diffuse_oren_nayar = require(glsl-diffuse-oren-nayar)
#pragma glslify: diffuse_lambert = require(glsl-diffuse-lambert)
#pragma glslify: diffuse_toon = require(./glsl-diffuse-toon.glsl);
#pragma glslify: specular_phong = require(glsl-specular-phong)
#pragma glslify: specular_blin_phong = require(glsl-specular-blinn-phong)
#pragma glslify: specular_beckmann = require(glsl-specular-beckmann)
#pragma glslify: specular_cook_torrance = require(glsl-specular-cook-torrance)
#pragma glslify: specular_ward = require(glsl-specular-ward)
#pragma glslify: specular_gaussian = require(glsl-specular-gaussian)

layout(std140, set = 2, binding = 0) buffer light_s {
	vec4 		position;
	vec4		direction;
	vec4 		ambient_color;
	vec4 		diffuse_color;
	vec4 		specular_color;
	float		inner_cone;
	float		outer_cone;
	float		constant;
	float		linear;
	float		quadratic;
	uint		type;
}	Light[];
layout (set = 2, binding = 1) uniform n_light_s {
	uint 		value;
}	n_light;
layout(set = 3, binding = 0) uniform material_s {
	vec4 		ambient_color;
	vec4 		diffuse_color;
	vec4 		specular_color;
	vec4 		emissive_color;
	vec4 		reflective_color;
	float 		shininess;
}	Material;
layout(set = 4, binding = 0) uniform sampler2D diffuse_samplerColor;

layout(location = 0) in vec3 in_frag_position_camera_space;
#ifdef F3D_ATTR_NORMAL_
layout(location = 1) in vec3 in_frag_normal_camera_space;
#endif
#ifdef F3D_ATTR_UV_
layout(location = 2) in vec2 in_frag_UV;
#endif

layout(location = 0) out vec4 outFragColor;

void main()
{
	vec4 diffuse_color = vec4(0.0, 0.0, 0.0, 0.0);
	vec4 ambient_color = vec4(0.0, 0.0, 0.0, 0.0);
	vec4 specular_color = vec4(0.0, 0.0, 0.0, 0.0);

	vec3 eye_direction = normalize(vec3(-in_frag_position_camera_space));
	#ifdef F3D_ATTR_NORMAL_
		vec3 n = normalize(inNormal);
	#endif

	for (uint i = 0; i < n_light.value; i++) {
		vec3 lightDir = normalize(vec3(Light[i].position) - in_frag_position_camera_space);
		vec4 spec = vec4(0.0);
		float dist = abs(length(lightDir));
		float angle =  diffuse_lambert(lightDir, n);
		float att = (1.0 / (Light[i].constant + 
							Light[i].linear * dist + 
							Light[i].quadratic * dist * dist));


		ambient_color += Material.ambient_color * Light[i].ambient_color;
		diffuse_color += Material.diffuse_color * texture(diffuse_samplerColor, inUV) * Light[i].diffuse_color * angle * att;
		specular_color += Material.specular_color * Light[i].specular_color * specular_phong(lightDir, eye, n, Material.shininess);
	}
	outFragColor = clamp(ambient_color + diffuse_color + specular_color, 0.0, 1.0);
	outFragColor.w = 1.0;
}
