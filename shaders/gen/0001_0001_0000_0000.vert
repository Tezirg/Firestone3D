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
layout(set = 2, binding = 0) buffer light_s {
	vec4 		position;
	vec4		direction;
	vec4 		ambient_color;
	vec4 		diffuse_color;
	vec4 		specular_color;
	float		inner_cone;
	float		outer_cone;
	float		constant;
	float		linear;
	float		quadratic;
	uint		type;
}	Light[];
layout (set = 2, binding = 1) uniform n_light_s {
	uint 		value;
}	n_light;

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 inNormal;
layout(location = 0) out vec4 outColor;

void main() 
{
	gl_Position = Camera.perspective * Camera.view * Mesh.model * position;
	
	vec4 normal = normalize(Mesh.model * inNormal);
	vec4 diffuse_color = vec4(0.0);
	for (int i = 0; i < n_light.value; i++) {
		vec3 l = vec3(normalize(Light[i].direction));
		float angle = max(dot(vec3(normal), l), 0.0);
		diffuse_color += Light[i].diffuse_color * vec4(1.000, 0.078, 0.576, 1.000) * angle;
	}
	outColor = clamp(diffuse_color, 0.0, 1.0);
	
	// GL->VK conventions
	gl_Position.y = -gl_Position.y;
	gl_Position.z = (gl_Position.z + gl_Position.w) / 2.0;
}
