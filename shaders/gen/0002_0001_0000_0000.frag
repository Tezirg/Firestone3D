#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
#define GLSLIFY 1

layout(set = 2, binding = 0) buffer light_s {
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

layout(location = 0) in vec4 normal;

layout(location = 0) out vec4 outFragColor;

void main() 
{
	vec4 diffuse_color = vec4(0.0);
	for (int i = 0; i < n_light.value; i++) {
		vec3 l = vec3(normalize(Light[i].direction));
		float angle = max(dot(vec3(normal), l), 0.0);
		diffuse_color += vec4(1.000, 0.078, 0.576, 1.000) * angle;
	}
	outFragColor = clamp(diffuse_color, 0.0, 1.0); 
}
