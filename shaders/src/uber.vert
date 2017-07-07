#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(std140, set = 0, binding = 0) uniform camera_s {
	mat4	view;
	mat4	perspective;
}   Camera;
layout(std140, set = 1, binding = 0) uniform mesh_s {
	mat4	model;
}	Mesh;


layout(location = 0) in vec4 in_vertex_position_local_space;
layout(location = 0) out vec4 out_vertex_position_camera_space;

#ifdef F3D_ATTR_NORMAL_
layout(location = 1) in vec4 in_vertex_normal_model_space;
layout(location = 1) out vec3 out_vertex_normal_camera_space;
#endif

#ifdef F3D_ATTR_UV_
layout(location = 2) in vec2 in_vertex_UV;
layout(location = 2) out vec2 out_vertex_UV;
#endif

void main() 
{
	out_vertex_position_camera_space = Camera.view * Mesh.model * in_vertex_position_local_space;
	
	#ifdef F3D_ATTR_NORMAL_
		mat4 normal = transpose(inverse(Mesh.model))
		outNormal = vec3(Camera.view * normal * inNormal);
	#endif

	#ifdef F3D_ATTR_UV_
		out_vertex_UV = in_vertex_UV;
	#endif
	

	gl_Position = Camera.perspective * Camera.view * Mesh.model * in_vertex_position_local_space;
	#ifdef F3D_VULKAN_TRANSFORM_
		gl_Position.y = -gl_Position.y;
	#endif
}
