#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
#extension GL_OES_standard_derivatives : enable
#define GLSLIFY 1

float phongSpecular(
  vec3 lightDirection,
  vec3 viewDirection,
  vec3 surfaceNormal,
  float shininess) {

  //Calculate Phong power
  vec3 R = -reflect(lightDirection, surfaceNormal);
  return pow(max(0.0, dot(viewDirection, R)), shininess);
}

float lambertDiffuse(
  vec3 lightDirection,
  vec3 surfaceNormal) {
  return max(0.0, dot(lightDirection, surfaceNormal));
}

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
	vec4 diffuse_color = vec4(0.0);
	vec4 ambient_color = vec4(0.0);
	vec4 specular_color = vec4(0.0);
	vec3 n = normalize(inNormal);
	vec3 e = normalize(inPosition);
	
	for (uint i = 0; i < n_light.value; i++) {
		vec3 l = normalize(vec3(Light[i].direction));
		float angle = lambertDiffuse(l, n);
		float spec = phongSpecular(l, e, n, Material.shininess);
		ambient_color += Material.ambient_color * Light[i].ambient_color;
		diffuse_color += Material.diffuse_color * Light[i].diffuse_color * angle;
		specular_color += Material.specular_color * Light[i].specular_color * spec;
	}
	outFragColor = clamp(ambient_color + diffuse_color + specular_color, 0.0, 1.0);
	outFragColor.w = 1.0;
}
