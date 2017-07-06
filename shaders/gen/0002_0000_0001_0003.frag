#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
#define GLSLIFY 1

layout(set = 2, binding = 0) uniform material_s {
	vec4 		ambient_color;
	vec4 		diffuse_color;
	vec4 		specular_color;
	vec4 		emissive_color;
	vec4 		reflective_color;
	float 		shininess;
}	Material;

layout(set = 3, binding = 0) uniform sampler2D samplerColor;

layout(location = 0) in vec2 inUv;

layout(location = 0) out vec4 outFragColor;

void main() 
{
	vec4 color = Material.ambient_color + Material.diffuse_color * texture(samplerColor, inUv);
	outFragColor = clamp(color, 0.0, 1.0);
}
