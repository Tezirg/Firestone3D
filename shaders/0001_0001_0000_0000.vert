#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(set = 0, binding = 0) uniform camera_s {
	mat4 	VP;
}   Camera;

layout(set = 1, binding = 0) uniform light_s {
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
}	Light[16];
layout (set = 1, binding = 1) uniform n_light_s {
	uint 		n_light;
}	n_light;

layout(set = 2, binding = 0) uniform model_s {
	mat4	M;
}	Model;

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 normal;

layout(location = 0) out vec3 color;

void main() 
{
	gl_Position = Camera.VP * Model.M * position;
	
	vec3 l = vec3(normalize(Light[0].direction));
	vec3 n = vec3(normalize(Model.M * normal));
	float angle = clamp(dot(n,l), 0.0, 1.0);
	color = vec3(1.000, 0.078, 0.576);
	color = color * angle;
	
   // GL->VK conventions
   gl_Position.y = -gl_Position.y;
   gl_Position.z = (gl_Position.z + gl_Position.w) / 2.0;
}
