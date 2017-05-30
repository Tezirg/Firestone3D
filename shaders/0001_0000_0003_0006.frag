#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(set = 3, binding = 0) uniform sampler2D diffuse_samplerColor;
layout(set = 4, binding = 0) uniform sampler2D specular_samplerColor;

layout(location = 0) in vec2 inUv;
layout(location = 1) in vec4 inColor;

layout(location = 0) out vec4 outFragColor;

void main() 
{
	vec4 texture_color = mix(texture(diffuse_samplerColor, inUv), texture(specular_samplerColor, inUv), 0.25);
	outFragColor = vec4(mix(texture_color, inColor, 0.05).rgb, 1.0);
}
