#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
#define GLSLIFY 1

layout(set = 2, binding = 0) uniform sampler2D diffuse_sampler;

layout(location = 0) in vec2 inUv;

layout(location = 0) out vec4 outFragColor;

void main() 
{
	outFragColor = texture(diffuse_sampler, inUv);
}
