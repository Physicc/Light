#version 330 core

layout (location = 0) in vec3 a_Position;

out vec3 v_texcoord;

uniform mat4 u_viewProjectionMatrix;

void main()
{
    v_texcoord = a_Position;
    gl_Position = u_viewProjectionMatrix * vec4(a_Position, 1.0);
} 