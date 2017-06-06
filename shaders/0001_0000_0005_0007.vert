#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(set = 0, binding = 0) uniform camera_s {
	mat4	view;
	mat4	perspective;
}   Camera;

layout(set = 1, binding = 0) uniform model_s {
	mat4	M;
}	Model;

layout(set = 2, binding = 0) uniform material_s {
	vec4 		ambient_color;
	vec4 		diffuse_color;
	vec4 		specular_color;
	vec4 		emissive_color;
	vec4 		reflective_color;
	float 		shininess;
}	Material;

layout(location = 0) in vec4 inPosition;
layout(location = 1) in vec4 inNormal;
layout(location = 2) in vec2 inUv;

layout(location = 0) out vec2 outUv;
layout(location = 1) out vec4 outColor;

void main() 
{
	gl_Position = Camera.perspective * Camera.view * Model.M * inPosition;
	
	outUv = inUv;
	outColor = mix(Material.ambient_color, Material.diffuse_color, 0.5);
	outColor = mix(outColor, Material.specular_color, 0.20);
	
   // GL->VK conventions
   gl_Position.y = -gl_Position.y;
   gl_Position.z = (gl_Position.z + gl_Position.w) / 2.0;
}
