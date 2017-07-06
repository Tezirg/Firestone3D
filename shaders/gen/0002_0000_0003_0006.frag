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
layout(set = 3, binding = 0) uniform sampler2D diffuse_samplerColor;
layout(set = 4, binding = 0) uniform sampler2D specular_samplerColor;

layout(location = 0) in vec2 inUv;
layout(location = 1) in vec4 inColor;

layout(location = 0) out vec4 outFragColor;

void main() 
{
	vec4 specular_color = Material.specular_color * texture(specular_samplerColor, inUv);
	vec4 diffuse_color = Material.diffuse_color * texture(diffuse_samplerColor, inUv);
	outFragColor = mix(diffuse_color, specular_color, 0.2);
}
