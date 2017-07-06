#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
#extension GL_OES_standard_derivatives : enable
#define GLSLIFY 1

layout(set = 0, binding = 0) uniform camera_s {
	mat4	view;
	mat4	perspective;
}   Camera;
layout(set = 1, binding = 0) uniform mesh_s {
	mat4	model;
}	Mesh;

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 inNormal;

layout(location = 0) out vec3 outNormal;
layout(location = 1) out vec3 outPosition;

void main() 
{
	mat4 viewModel = Camera.view * Mesh.model;
	mat3 normalMatrix = transpose(inverse(mat3(Mesh.model)));
	outNormal = normalize(normalMatrix * vec3(inNormal));
	outPosition = normalize( - vec3(viewModel * position));
	
	gl_Position = Camera.perspective * Camera.view * Mesh.model * position;
	
	// GL->VK conventions
	gl_Position.y = -gl_Position.y;
	gl_Position.z = (gl_Position.z + gl_Position.w) / 2.0;
}
