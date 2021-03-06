#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(set = 0, binding = 0) uniform camera_s {
	mat4 	VP;
}   Camera;

layout(set = 1, binding = 0) uniform model_s {
	mat4	M;
}	Model;

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 normal;

layout(location = 0) out vec3 color;

void main() 
{
	gl_Position = Camera.VP * Model.M * position;
	
	color = vec3(0.0, 0.0, 0.8);
	
   // GL->VK conventions
   gl_Position.y = -gl_Position.y;
   gl_Position.z = (gl_Position.z + gl_Position.w) / 2.0;
}
