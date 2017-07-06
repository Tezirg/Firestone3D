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
	vec4 specular_color = vec4(0.0, 0.0, 0.0, 0.0);
	vec3 n = normalize(inNormal);
	vec3 e = normalize(-inPosition);
	
	for (uint i = 0; i < n_light.value; i++) {
		vec3 l = vec3(normalize(Light[i].direction));
		float angle = max(dot(n, l), 0.0);
		float spec_intensisty = 0.0;
		if (angle > 0.0) {
			vec3 h = normalize(l + e);
			float intSpec = max(dot(h, n), 0.0);
			spec_intensisty = pow(intSpec, Material.shininess);
		}
		ambient_color += Material.ambient_color * Light[i].ambient_color;
		diffuse_color += Material.diffuse_color * Light[i].diffuse_color * angle;
		specular_color += Material.specular_color * Light[i].specular_color * spec_intensisty;
	}
	outFragColor = clamp(diffuse_color + specular_color + ambient_color, 0.0, 1.0);
}
