#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

#ifndef F3D_DECLARATIONS
#define F3D_DECLARATIONS

#define F3D_REPLACED_DECLARATIONS

#endif //F3D_DECLARATIONS

#ifdef F3D_UNIFORM_CAMERA
layout(std140, set = 0, binding = 0) uniform camera_s {
	mat4	view;
	mat4	perspective;
}   Camera;
#endif
#ifdef F3D_UNIFORM_MODEL
layout(std140, set = 1, binding = 0) uniform mesh_s {
	mat4	model;
}	Mesh;
#endif

#ifdef F3D_ATTR_POSITION
layout(location = 0) in vec4 in_vertex_position_local_space;
layout(location = 0) out vec3 out_vertex_position_world_space;
#endif
#ifdef F3D_ATTR_NORMAL
layout(location = 1) in vec4 in_vertex_normal_local_space;
layout(location = 1) out vec3 out_vertex_normal_world_space;
#endif
#ifdef F3D_ATTR_COLOR
layout (location = 2) in vec4 in_vertex_color;
layout (location = 2) out vec4 out_vertex_color;
#endif
#ifdef F3D_ATTR_UV
layout(location = 3) in vec2 in_vertex_UV;
layout(location = 3) out vec2 out_vertex_UV;
#endif
#ifdef F3D_UNIFORM_CAMERA
layout (location = 10) out vec3 out_camera_position_world_space;
#endif

void main() 
{
	#if F3D_ATTR_POSITION && F3D_UNIFORM_CAMERA && F3D_UNIFORM_MODEL
		out_vertex_position_world_space = vec3(Mesh.model * in_vertex_position_local_space);
		gl_Position = Camera.perspective * Camera.view * Mesh.model * in_vertex_position_local_space;
	#elif F3D_ATTR_POSITION && F3D_UNIFORM_CAMERA
		out_vertex_position_world_space = vec3(in_vertex_position_local_space);
		gl_Position = Camera.perspective * Camera.view * in_vertex_position_local_space;
	#elif F3D_ATTR_POSITION && F3D_UNIFORM_MODEL
		out_vertex_position_world_space = vec3(out_vertex_position_camera_space);
		gl_Position = out_vertex_position_camera_space;
	#elif F3D_ATTR_POSITION
		out_vertex_position_world_space = vec3(in_vertex_position_local_space);
		gl_Position = in_vertex_position_local_space;
	#elif F3D_UNIFORM_CAMERA && F3D_UNIFORM_MODEL
		gl_Position = Camera.perspective * Camera.view * Mesh.model * vec4(0.0, 0.0, 0.0, 1.0);
	#elif F3D_UNIFORM_CAMERA
		gl_Position = Camera.perspective * Camera.view * vec4(0.0, 0.0, 0.0, 1.0);
	#elif F3D_UNIFORM_MODEL
		gl_Position = Mesh.model * vec4(0.0, 0.0, 0.0, 1.0);
	#else
		gl_Position = vec4(0.0, 0.0, 0.0, 1.0);
	#endif
	
	#if F3D_ATTR_NORMAL && F3D_UNIFORM_CAMERA && F3D_UNIFORM_MODEL
		mat4 world_normal = transpose(inverse(Mesh.model));
		out_vertex_normal_world_space = vec3(world_normal * in_vertex_normal_local_space);
	#elif F3D_ATTR_NORMAL && F3D_UNIFORM_MODEL
		mat4 normal = transpose(inverse(Mesh.model));
		out_vertex_normal_world_space = vec3(normal * in_vertex_normal_local_space);
	#elif F3D_ATTR_NORMAL && F3D_UNIFORM_CAMERA
		mat4 camera_normal = transpose(inverse(Camera.view));
		out_vertex_normal_world_space = vec3(in_vertex_normal_local_space);
	#elif F3D_ATTR_NORMAL
		out_vertex_normal_world_space = vec3(in_vertex_normal_local_space);
	#endif
	
	#ifdef F3D_UNIFORM_CAMERA
		out_camera_position_world_space = Camera.view[2].xyz;
	#endif
	
	#ifdef F3D_ATTR_COLOR
		out_vertex_color = in_vertex_color;
	#endif
	#ifdef F3D_ATTR_UV
		out_vertex_UV = in_vertex_UV;
	#endif
	
	#ifdef F3D_VULKAN_TRANSFORM_Y
		gl_Position.y = -gl_Position.y;
	#endif
	#ifdef F3D_VULKAN_TRANSFORM_Z
		gl_Position.z = (gl_Position.z + gl_Position.w) / 2.0;
	#endif
}
