#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
#extension GL_OES_standard_derivatives : enable
#define GLSLIFY 1

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

float orenNayarDiffuse(
  vec3 lightDirection,
  vec3 viewDirection,
  vec3 surfaceNormal,
  float roughness,
  float albedo) {
  
  float LdotV = dot(lightDirection, viewDirection);
  float NdotL = dot(lightDirection, surfaceNormal);
  float NdotV = dot(surfaceNormal, viewDirection);

  float s = LdotV - NdotL * NdotV;
  float t = mix(1.0, max(NdotL, NdotV), step(0.0, s));

  float sigma2 = roughness * roughness;
  float A = 1.0 + sigma2 * (albedo / (sigma2 + 0.13) + 0.5 / (sigma2 + 0.33));
  float B = 0.45 * sigma2 / (sigma2 + 0.09);

  return albedo * max(0.0, NdotL) * (A + B * s / t) / M_PI;
}

float lambertDiffuse(
  vec3 lightDirection,
  vec3 surfaceNormal) {
  return max(0.0, dot(lightDirection, surfaceNormal));
}

float toonDiffuse(
  vec3 lightDirection,
  vec3 surfaceNormal,
  int nSteps) {
  float fSteps = float(abs(nSteps));
  float angle = max(0.0, dot(lightDirection, surfaceNormal));
  return round(angle * fSteps) / fSteps;
}

float phongSpecular(
  vec3 lightDirection,
  vec3 viewDirection,
  vec3 surfaceNormal,
  float shininess) {

  //Calculate Phong power
  vec3 R = -reflect(lightDirection, surfaceNormal);
  return pow(max(0.0, dot(viewDirection, R)), shininess);
}

float blinnPhongSpecular(
  vec3 lightDirection,
  vec3 viewDirection,
  vec3 surfaceNormal,
  float shininess) {

  //Calculate Blinn-Phong power
  vec3 H = normalize(viewDirection + lightDirection);
  return pow(max(0.0, dot(surfaceNormal, H)), shininess);
}

float beckmannDistribution_0(float x, float roughness) {
  float NdotH = max(x, 0.0001);
  float cos2Alpha = NdotH * NdotH;
  float tan2Alpha = (cos2Alpha - 1.0) / cos2Alpha;
  float roughness2 = roughness * roughness;
  float denom = 3.141592653589793 * roughness2 * cos2Alpha * cos2Alpha;
  return exp(tan2Alpha / roughness2) / denom;
}

float beckmannSpecular(
  vec3 lightDirection,
  vec3 viewDirection,
  vec3 surfaceNormal,
  float roughness) {
  return beckmannDistribution_0(dot(surfaceNormal, normalize(lightDirection + viewDirection)), roughness);
}

float beckmannDistribution_1(float x, float roughness) {
  float NdotH = max(x, 0.0001);
  float cos2Alpha = NdotH * NdotH;
  float tan2Alpha = (cos2Alpha - 1.0) / cos2Alpha;
  float roughness2 = roughness * roughness;
  float denom = 3.141592653589793 * roughness2 * cos2Alpha * cos2Alpha;
  return exp(tan2Alpha / roughness2) / denom;
}

float cookTorranceSpecular(
  vec3 lightDirection,
  vec3 viewDirection,
  vec3 surfaceNormal,
  float roughness,
  float fresnel) {

  float VdotN = max(dot(viewDirection, surfaceNormal), 0.0);
  float LdotN = max(dot(lightDirection, surfaceNormal), 0.0);

  //Half angle vector
  vec3 H = normalize(lightDirection + viewDirection);

  //Geometric term
  float NdotH = max(dot(surfaceNormal, H), 0.0);
  float VdotH = max(dot(viewDirection, H), 0.000001);
  float LdotH = max(dot(lightDirection, H), 0.000001);
  float G1 = (2.0 * NdotH * VdotN) / VdotH;
  float G2 = (2.0 * NdotH * LdotN) / LdotH;
  float G = min(1.0, min(G1, G2));
  
  //Distribution term
  float D = beckmannDistribution_1(NdotH, roughness);

  //Fresnel term
  float F = pow(1.0 - VdotN, fresnel);

  //Multiply terms and done
  return  G * F * D / max(3.14159265 * VdotN, 0.000001);
}

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

float wardSpecular(
  vec3 lightDirection,
  vec3 viewDirection,
  vec3 surfaceNormal,
  vec3 fiberParallel,
  vec3 fiberPerpendicular,
  float shinyParallel,
  float shinyPerpendicular) {

  float NdotL = dot(surfaceNormal, lightDirection);
  float NdotR = dot(surfaceNormal, viewDirection);

  if(NdotL < 0.0 || NdotR < 0.0) {
    return 0.0;
  }

  vec3 H = normalize(lightDirection + viewDirection);

  float NdotH = dot(surfaceNormal, H);
  float XdotH = dot(fiberParallel, H);
  float YdotH = dot(fiberPerpendicular, H);

  float coeff = sqrt(NdotL/NdotR) / (4.0 * M_PI * shinyParallel * shinyPerpendicular); 
  float theta = (pow(XdotH/shinyParallel, 2.0) + pow(YdotH/shinyPerpendicular, 2.0)) / (1.0 + NdotH);

  return coeff * exp(-2.0 * theta);
}

float gaussianSpecular(
  vec3 lightDirection,
  vec3 viewDirection,
  vec3 surfaceNormal,
  float shininess) {
  vec3 H = normalize(lightDirection + viewDirection);
  float theta = acos(dot(H, surfaceNormal));
  float w = theta / shininess;
  return exp(-w*w);
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

layout(location = 0) in vec3 in_frag_position_camera_space;
#ifdef F3D_ATTR_NORMAL_
layout(location = 1) in vec3 in_frag_normal_camera_space;
#endif
#ifdef F3D_ATTR_UV_
layout(location = 2) in vec2 in_frag_UV;
#endif

layout(location = 0) out vec4 outFragColor;

void main()
{
	vec4 diffuse_color = vec4(0.0, 0.0, 0.0, 0.0);
	vec4 ambient_color = vec4(0.0, 0.0, 0.0, 0.0);
	vec4 specular_color = vec4(0.0, 0.0, 0.0, 0.0);

	vec3 eye_direction = normalize(vec3(-in_frag_position_camera_space));
	#ifdef F3D_ATTR_NORMAL_
		vec3 n = normalize(inNormal);
	#endif

	for (uint i = 0; i < n_light.value; i++) {
		vec3 lightDir = normalize(vec3(Light[i].position) - in_frag_position_camera_space);
		vec4 spec = vec4(0.0);
		float dist = abs(length(lightDir));
		float angle =  lambertDiffuse(lightDir, n);
		float att = (1.0 / (Light[i].constant + 
							Light[i].linear * dist + 
							Light[i].quadratic * dist * dist));

		ambient_color += Material.ambient_color * Light[i].ambient_color;
		diffuse_color += Material.diffuse_color * texture(diffuse_samplerColor, inUV) * Light[i].diffuse_color * angle * att;
		specular_color += Material.specular_color * Light[i].specular_color * phongSpecular(lightDir, eye, n, Material.shininess);
	}
	outFragColor = clamp(ambient_color + diffuse_color + specular_color, 0.0, 1.0);
	outFragColor.w = 1.0;
}
