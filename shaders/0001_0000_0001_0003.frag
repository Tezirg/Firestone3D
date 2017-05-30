#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(set = 3, binding = 0) uniform sampler2D samplerColor;

layout(location = 0) in vec2 inUv;
layout(location = 1) in vec4 inColor;

layout(location = 0) out vec4 outFragColor;

void main() 
{
	vec4 color = mix(texture(samplerColor, inUv), inColor, 0.05);
	outFragColor = vec4(color.rgb, 1.0);
}
