#ifndef OBJECT_LOADER_SHADERS_UNIFORM_MATRICES_GLSL
#define OBJECT_LOADER_SHADERS_UNIFORM_MATRICES_GLSL

layout(std140, binding = UNIFORM_MATRICES_BINDING) uniform Matrices
{
    mat4 projection;
    mat4 view;
};

uniform mat4 model;

#endif
