#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
#extension GL_OES_standard_derivatives : enable

#ifndef F3D_DECLARATIONS
#define F3D_DECLARATIONS

#define F3D_REPLACED_DECLARATIONS

#endif //F3D_DECLARATIONS

#ifdef F3D_SHADING_DIFFUSE_LAMBERT
	#pragma glslify: diffuse_lambert = require(glsl-diffuse-lambert)
#endif
#ifdef F3D_SHADING_DIFFUSE_ORENNAYAR
	#pragma glslify: diffuse_oren_nayar = require(glsl-diffuse-oren-nayar)
#endif
#ifdef F3D_SHADING_DIFFUSE_TOON
	#pragma glslify: diffuse_toon = require(./glsl-diffuse-toon.glsl);
#endif
#ifdef F3D_SHADING_SPECULAR_PHONG
	#pragma glslify: specular_phong = require(glsl-specular-phong)
#endif
#ifdef F3D_SHADING_SPECULAR_BLINN_PHONG
	#pragma glslify: specular_blinn_phong = require(glsl-specular-blinn-phong)
#endif
#ifdef F3D_SHADING_SPECULAR_BECKMANN
	#pragma glslify: specular_beckmann = require(glsl-specular-beckmann)
#endif
#ifdef F3D_SHADING_SPECULAR_COOK_TORRANCE
	#pragma glslify: specular_cook_torrance = require(glsl-specular-cook-torrance)
#endif
#ifdef F3D_SHADING_SPECULAR_WARD
	#pragma glslify: specular_ward = require(glsl-specular-ward)
#endif
#ifdef F3D_SHADING_SPECULAR_GAUSSIAN
	#pragma glslify: specular_gaussian = require(glsl-specular-gaussian)
#endif

#ifdef F3D_UNIFORM_LIGHT
layout(std140, set = 2, binding = 0) buffer light_s {
	vec4 		position;
	vec4		direction;
	vec4 		ambient_color;
	vec4 		diffuse_color;
	vec4 		specular_color;
	float		spot_exponent;
	float		spot_cutoff;
	float		spot_cos_cutoff;
	float		constant;
	float		linear;
	float		quadratic;
	uint		type;
	uint		padding_std140;
}	Light[];
layout (set = 2, binding = 1) uniform n_light_s {
	uint 		value;
}	light_count;
#endif

#ifdef F3D_UNIFORM_MATERIAL
layout(set = 3, binding = 0) uniform material_s {
	vec4 		ambient_color;
	vec4 		diffuse_color;
	vec4 		specular_color;
	vec4 		emissive_color;
	vec4 		reflective_color;
	float 		shininess;
	float		roughness;	
}	Material;
#endif

#ifdef F3D_UNIFORM_SAMPLER_AMBIENT
layout(set = 4, binding = 0) uniform sampler2D ambient_sampler;
#endif
#ifdef F3D_UNIFORM_SAMPLER_DIFFUSE
layout(set = 5, binding = 0) uniform sampler2D diffuse_sampler;
#endif
#ifdef F3D_UNIFORM_SAMPLER_SPECULAR
layout(set = 6, binding = 0) uniform sampler2D specular_sampler;
#endif
#ifdef F3D_UNIFORM_SAMPLER_EMISSIVE
layout(set = 7, binding = 0) uniform sampler2D emmissive_sampler;
#endif
#ifdef F3D_UNIFORM_SAMPLER_HEIGHT
layout(set = 8, binding = 0) uniform sampler2D height_sampler;
#endif
#ifdef F3D_UNIFORM_SAMPLER_NORMALS
layout(set = 9, binding = 0) uniform sampler2D normals_sampler;
#endif
#ifdef F3D_UNIFORM_SAMPLER_DISPLACEMENT
layout(set = 10, binding = 0) uniform sampler2D displacement_sampler;
#endif
#ifdef F3D_UNIFORM_SAMPLER_REFLECTION
layout(set = 11, binding = 0) uniform sampler2D reflection_sampler;
#endif

#ifdef F3D_ATTR_POSITION
layout(location = 0) in vec3 in_frag_position_camera_space;
#endif
#ifdef F3D_ATTR_NORMAL
layout(location = 1) in vec3 in_frag_normal_camera_space;
#endif
#ifdef F3D_ATTR_COLOR
layout (location = 2) in vec4 in_frag_color;
#endif
#ifdef F3D_ATTR_UV
layout(location = 3) in vec2 in_frag_UV;
#endif

layout(location = 0) out vec4 out_frag_color;

#ifdef F3D_UNIFORM_LIGHT
	vec3	computeLightDirection(in uint lightIndex, 
								  in vec3 eyeDirection)
	{
		vec3 ld = vec3(0.0);
		switch (Light[lightIndex].type) 
		{
		#ifdef F3D_LIGHT_DIRECTIONAL
			case 1: //F3D_LIGHT_DIRECTIONAL_
				ld = vec3(Light[lightIndex].direction);
				break;
		#endif
		#ifdef F3D_LIGHT_POINT
			case 2: //F3D_LIGHT_POINT
				ld = vec3(normalize(vec3(Light[lightIndex].position)) + normalize(eyeDirection));
				break;
		#endif
		#ifdef F3D_LIGHT_SPOT
			case 3: //F3D_LIGHT_SPOT
				ld = vec3(normalize(vec3(Light[lightIndex].position)) + normalize(eyeDirection));
				break;
		#endif
			default:
				break;
		}
		return normalize(ld);
	}

	float 		computeAttenuation(in uint lightIndex,
								   in vec3 lightDirection)
	{
		float att = 0.0;
		switch (Light[lightIndex].type)
		{
		#ifdef F3D_LIGHT_DIRECTIONAL
			case 1: //Directionnal
				att = 1.0;
				break;
		#endif
		#ifdef F3D_LIGHT_POINT
			case 2: //Point
				float dist = abs(length(lightDirection));
				att = 1.0 / (Light[lightIndex].constant + 
							 Light[lightIndex].linear * dist + 
							 Light[lightIndex].quadratic * dist * dist);
				break;
		#endif
		#ifdef F3D_LIGHT_SPOT
			case 3: //Spot
				float spotEffect = dot(normalize(vec3(Light[lightIndex].direction)), normalize(-lightDirection));
				if (spotEffect > Light[lightIndex].spot_cos_cutoff) {
					spotEffect = pow(spotEffect, Light[lightIndex].spot_exponent);
					att = spotEffect / (Light[lightIndex].constant + 
										Light[lightIndex].linear * dist + 
										Light[lightIndex].quadratic * dist * dist);
				}
				break;
		#endif
			default:
				break;
		}
		return att;
	}
	
	vec4		computeAmbient(in uint lightIndex)
	{
		#ifdef F3D_COLOR_AMBIENT
			vec4 ambient = Light[lightIndex].ambient_color;
			#ifdef F3D_UNIFORM_MATERIAL
				ambient *= Material.ambient_color;
			#endif
			#if F3D_UNIFORM_SAMPLER_AMBIENT && F3D_ATTR_UV
				ambient *= texture(ambient_sampler, in_frag_UV);
			#endif
		#else
			vec4 ambient = vec4(0.0, 0.0, 0.0, 1.0);
		#endif
		return ambient;
	}

	vec4		computeDiffuse(in uint lightIndex, 
							   in vec3 lightDirection, 
							   in vec3 viewDirection, 
							   in vec3 surfaceNormal)
	{
		#ifdef F3D_COLOR_DIFFUSE
			vec4 diffuse = Light[lightIndex].diffuse_color;
			#if F3D_UNIFORM_MATERIAL
				diffuse *= Material.diffuse_color;
			#endif
			#if F3D_UNIFORM_SAMPLER_DIFFUSE && F3D_ATTR_UV
				diffuse *= texture(diffuse_sampler, in_frag_UV);
			#endif
			
			#if F3D_SHADING_DIFFUSE_LAMBERT
				diffuse *= diffuse_lambert(lightDirection, surfaceNormal);
			#elif F3D_SHADING_DIFFUSE_TOON
				diffuse *= diffuse_toon(lightDirection, surfaceNormal);
			#elif F3D_SHADING_DIFFUSE_ORENNAYAR
				float roughness = 0.5;
				#if F3D_UNIFORM_MATERIAL_
					roughness = Material.roughness;
				#endif
				diffuse *= diffuse_oren_nayar(lightDirection, viewDirection, surfaceNormal, roughness, 1.0);
			#endif
			
		#else
			vec4 diffuse = vec4(0.0, 0.0, 0.0, 1.0);
		#endif
		return diffuse;
	}
	
	vec4		computeSpecular(in uint lightIndex, 
								in vec3 lightDirection, 
								in vec3 viewDirection, 
								in vec3 surfaceNormal)
	{
		#ifdef F3D_COLOR_SPECULAR
			float shininess = 1.0;
			float roughness = 1.0;
			vec4 specular = Light[lightIndex].specular_color;
			#if F3D_UNIFORM_MATERIAL
				specular *= Material.specular_color;
				shininess = Material.shininess;
				roughness = Material.roughness;
			#endif
			#if F3D_UNIFORM_SAMPLER_SPECULAR && F3D_ATTR_UV
				specular *= texture(specular_sampler, in_frag_UV);
			#endif
			
			#if F3D_SHADING_SPECULAR_PHONG
				specular *= specular_phong(lightDirection, viewDirection, surfaceNormal, shininess);
			#elif F3D_SHADING_SPECULAR_BLINN_PHONG
				specular *= specular_blinn_phong(lightDirection, viewDirection, surfaceNormal, shininess);
			#elif F3D_SHADING_SPECULAR_BECKMANN
				specular *= specular_beckmann(lightDirection, viewDirection, surfaceNormal, roughness);
			#elif F3D_SHADING_SPECULAR_COOK_TORRANCE
				specular *= specular_cook_torrance(lightDirection, viewDirection, surfaceNormal, roughness, 1.0);
			#elif F3D_SHADING_SPECULAR_WARD && F3D_ATTR_UV
			    vec3 fiber = normalize(vec3(in_frag_UV, 0.0) - dot(vec3(in_frag_UV, 0.0), surfaceNormal) * surfaceNormal);
				vec3 perp = normalize(cross(fiber, normal));
				specular *= specular_ward(lightDirection, viewDirection, surfaceNormal, fiber, perp, 0.1, roughness);
			#elif F3D_SHADING_SPECULAR_GAUSSIAN
				specular *= specular_gaussian(lightDirection, viewDirection, surfaceNormal, shininess);
			#endif
		#else
			vec4 specular = vec4(0.0, 0.0, 0.0, 1.0);
		#endif
		return specular;
	}
#endif

void main()
{
	#ifdef F3D_COLOR_AMBIENT
		vec4 ambient_color = vec4(0.0, 0.0, 0.0, 0.0);
	#endif
	#ifdef F3D_COLOR_DIFFUSE
		vec4 diffuse_color = vec4(0.0, 0.0, 0.0, 0.0);
	#endif
	#ifdef F3D_COLOR_SPECULAR
		vec4 specular_color = vec4(0.0, 0.0, 0.0, 0.0);
	#endif
	#ifdef F3D_COLOR_EMISSIVE
		#ifdef F3D_UNIFORM_MATERIAL
			vec4 emissive_color = Material.emissive_color;
		#else
			vec4 emissive_color = vec4(0.0, 0.0, 0.0, 0.0);
		#endif
	#endif

	#ifdef F3D_ATTR_NORMAL
		vec3 surfaceNormal = in_frag_normal_camera_space;
		#if F3D_UNIFORM_SAMPLER_NORMALS && F3D_ATTR_UV
			surfaceNormal += texture(normals_sampler, in_frag_UV).xyz;
		#endif
		surfaceNormal = normalize(surfaceNormal);
	#else
		vec3 surfaceNormal = vec3(0.0, 1.0, 0.0);
	#endif
	#ifdef F3D_ATTR_POSITION
		vec3 viewDirection = vec3(-in_frag_position_camera_space);
		#if F3D_UNIFORM_SAMPLER_HEIGHT && F3D_ATTR_UV
			viewDirection += surfaceNormal * texture(height_sampler, in_frag_UV).xyz;
		#endif
		viewDirection = normalize(viewDirection);
	#else
		vec3 viewDirection = vec3(0.0, 0.0, 1.0);
	#endif


	#if F3D_UNIFORM_LIGHT
		for (uint i = 0; i < light_count.value; i++) {
			vec3 lightDirection = computeLightDirection(i, viewDirection);
			float attenuation = computeAttenuation(i, lightDirection);
			
			#ifdef F3D_COLOR_AMBIENT
				ambient_color += computeAmbient(i) * attenuation;
			#endif
			#ifdef F3D_COLOR_DIFFUSE
				diffuse_color += computeDiffuse(i, lightDirection, viewDirection, surfaceNormal) * attenuation;
			#endif
			#ifdef F3D_COLOR_SPECULAR
				specular_color += computeSpecular(i, lightDirection, viewDirection, surfaceNormal) * attenuation;
			#endif
		}
	#endif
	
	#ifdef F3D_ATTR_COLOR
		out_frag_color += in_frag_color;
	#endif
	#ifdef F3D_COLOR_AMBIENT
		out_frag_color += ambient_color;
	#endif
	#ifdef F3D_COLOR_DIFFUSE
		out_frag_color += diffuse_color;
	#endif
	#ifdef F3D_COLOR_SPECULAR
		out_frag_color += specular_color;
	#endif
	#ifdef F3D_COLOR_EMISSIVE
		out_frag_color += emissive_color;
	#endif
	
	out_frag_color = clamp(out_frag_color, 0.0, 1.0);
	out_frag_color.w = 1.0;
}
