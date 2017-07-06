#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(std140, set = 2, binding = 0) buffer light_s {
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
layout(set = 2, binding = 1) uniform n_light_s {
	uint 		value;
}	n_light;
layout(set = 3, binding = 0) uniform material_s {
	vec4 		ambient_color;
	vec4 		diffuse_color;
	vec4 		specular_color;
	vec4 		emissive_color;
	vec4 		reflective_color;
	float 		shininess;
}	Material;

layout(location = 0) in vec3 inNormal;
layout(location = 1) in vec3 inPosition;

layout(location = 0) out vec4 outFragColor;

void main()
{
	vec4 diffuse_color = vec4(0.0, 0.0, 0.0, 0.0);
	vec4 ambient_color = vec4(0.0, 0.0, 0.0, 0.0);
	for (uint i = 0; i < n_light.value; i++) {
		vec3 L = inPosition - vec3(Light[i].position);
		float distance = abs(length(L));
		float angle = max(dot(normalize(inNormal), normalize(L)), 0.0);
		diffuse_color += Material.diffuse_color * Light[i].diffuse_color * angle;
		ambient_color += Material.ambient_color * Light[i].ambient_color;
	}
	outFragColor = clamp(ambient_color + diffuse_color, 0.0, 1.0);
}
