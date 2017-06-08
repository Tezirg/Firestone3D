#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(set = 0, binding = 0) uniform camera_s {
	mat4	view;
	mat4	perspective;
}   Camera;

layout(set = 1, binding = 0) uniform mesh_s {
	mat4	model;
}	Mesh;

layout(location = 0) in vec4 position;

void main() 
{
	gl_Position = Camera.perspective * Camera.view * Mesh.model * position;
	
	// GL->VK conventions
	gl_Position.y = -gl_Position.y;
	gl_Position.z = (gl_Position.z + gl_Position.w) / 2.0;
}
