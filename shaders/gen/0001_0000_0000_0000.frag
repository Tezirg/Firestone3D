#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
#define GLSLIFY 1

layout(location = 0) out vec4 FragColor;

void main() 
{
   FragColor = vec4(1.000, 0.078, 0.576, 1.000); //Deep pink
}
