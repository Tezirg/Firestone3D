#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 normal;

layout(std140, binding = 0) uniform buf {
	mat4 	MVP;
}   uniformBuf;

layout (location = 0) out vec3 color;

void main() 
{
	vec4 vertex_Position = uniformBuf.MVP * position;
	
	color = vec3(0.0, 0.804, 0.0);
	gl_Position = vertex_Position;
	
   // GL->VK conventions
   //gl_Position.y = -gl_Position.y;
   //gl_Position.z = (gl_Position.z + gl_Position.w) / 2.0;
}
