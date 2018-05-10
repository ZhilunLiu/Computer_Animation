#version 430 core

#include "LayoutLocations.glsl"

layout(location = VERTICES_LAYOUT_LOCATION) in vec3 position;
layout(location = NORMALS_LAYOUT_LOCATION) in vec3 normal;
layout(location = TEX_COORDS_LAYOUT_LOCATION) in vec2 texCoord;

#include "UniformMatrices.glsl"

out VertexData
{
    vec3 position;
    vec3 normal;
    vec2 texCoord;
    vec3 eyeDirection;
    vec3 lightDirection;
    vec3 lightPosition;
} outData;

const vec3 Light = vec3(0, 5, 0);

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0);

    // Vertex position in world-space.
    outData.position = vec3(model * vec4(position, 1.0));

    // Vertex to camera.
    vec3 vertexPos = (view * model * vec4(position, 1.0)).xyz;
    outData.eyeDirection = vec3(0, 0, 0) - vertexPos;

    // Vertex to light.
    vec3 lightPos = (view * vec4(Light, 1.0)).xyz;
    outData.lightDirection = outData.eyeDirection;

    // Vertex normal.
    outData.normal = (view * model * vec4(normal, 0)).xyz;
    outData.lightPosition = Light;
    outData.texCoord = texCoord;
}
