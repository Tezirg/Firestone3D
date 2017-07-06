#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
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
layout(location = 2) in vec2 inUV;

layout(location = 0) out vec3 outPosition;
layout(location = 1) out vec3 outNormal;
layout(location = 2) out vec2 outUV;

const mat4 vk_clip = mat4(1.0f,  0.0f, 0.0f, 0.0f,
						  0.0f, -1.0f, 0.0f, 0.0f,
						  0.0f,  0.0f, 0.5f, 0.0f,
						  0.0f,  0.0f, 0.5f, 1.0f);

void main() 
{
	mat3 normalMatrix = transpose(inverse(mat3(Mesh.model)));
	outPosition = vec3(Camera.view * Mesh.model * position);
	outNormal = vec3(normalMatrix * vec3(inNormal));
	outUV = inUV;

	gl_Position = vk_clip * Camera.perspective * Camera.view * Mesh.model * position;
	
	
}
