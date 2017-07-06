#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

#pragma glslify: phong = require(glsl-specular-phong)

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
layout (set = 2, binding = 1) uniform n_light_s {
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
layout(set = 4, binding = 0) uniform sampler2D diffuse_samplerColor;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inUV;

layout(location = 0) out vec4 outFragColor;

void main()
{
	vec4 diffuse_color = vec4(0.0, 0.0, 0.0, 0.0);
	vec4 ambient_color = vec4(0.0, 0.0, 0.0, 0.0);
	vec4 specular_color = vec4(0.0, 0.0, 0.0, 0.0);

	vec3 n = normalize(inNormal);
	vec3 eye = normalize(vec3(-inPosition));
	for (uint i = 0; i < n_light.value; i++) {
		vec3 lightDir = normalize(vec3(Light[i].position) - inPosition);
		vec4 spec = vec4(0.0);
		float dist = abs(length(lightDir));
		float angle = max(dot(n, lightDir), 0.0);
		float att = (1.0 / (Light[i].constant + 
							Light[i].linear * dist + 
							Light[i].quadratic * dist * dist));


		ambient_color += Material.ambient_color * Light[i].ambient_color;
		diffuse_color += Material.diffuse_color * texture(diffuse_samplerColor, inUV) * Light[i].diffuse_color * angle;
		specular_color += Material.specular_color * Light[i].specular_color * phong(lightDir, eye, n, Material.shininess);
	}
	outFragColor = clamp(ambient_color + diffuse_color + specular_color, 0.0, 1.0);
}
