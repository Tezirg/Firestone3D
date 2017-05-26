#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(std140) uniform;

layout(set = 0, binding = 0) uniform camera_s {
	mat4 	VP;
}   Camera;

layout(set = 1, binding = 0) uniform model_s {
	mat4	M;
}	Model;

layout(location = 0) in vec4 inPosition;
layout(location = 1) in vec4 inNormal;
layout(location = 2) in vec2 inUv;

layout(location = 0) out vec2 outUv;

void main() 
{
	gl_Position = Camera.VP * Model.M * inPosition;
	
	outUv = inUv;
	
   // GL->VK conventions
   gl_Position.y = -gl_Position.y;
   gl_Position.z = (gl_Position.z + gl_Position.w) / 2.0;
}
