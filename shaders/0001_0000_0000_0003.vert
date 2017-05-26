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

layout(set = 2, binding = 0) uniform material_s {
	vec3 		ambient_color;
	vec3 		diffuse_color;
	vec3 		specular_color;
	vec3 		emissive_color;
	vec3 		reflective_color;
	float 		shininess;
}	Material;

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 normal;
layout(location = 0) out vec3 color;

void main() 
{
	gl_Position = Camera.VP * Model.M * position;
	
	color = mix(Material.diffuse_color, Material.ambient_color, 0.5);
	
   // GL->VK conventions
   gl_Position.y = -gl_Position.y;
   gl_Position.z = (gl_Position.z + gl_Position.w) / 2.0;
}
