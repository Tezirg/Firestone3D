#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(set = 4, binding = 0) uniform sampler2D diffuse_samplerColor;

layout(location = 0) in vec4 in_ambient_color;
layout(location = 1) in vec4 in_diffuse_color;
layout(location = 2) in vec4 in_specular_color;
layout(location = 3) in vec2 inUV;

layout(location = 0) out vec4 outFragColor;

void main() 
{
	vec4 diffuse_color = in_diffuse_color * texture(diffuse_samplerColor, inUV);

	outFragColor = in_ambient_color + diffuse_color + in_specular_color;
}
