#type vertex
#version 330 core

layout (location = 0) in vec3 a_Position;

out vec3 v_texcoord;

uniform mat4 u_viewProjectionMatrix;

void main()
{
    v_texcoord = a_Position;
    gl_Position = u_viewProjectionMatrix * vec4(a_Position, 1.0);
}

#type fragment
#version 330 core

in vec3 v_texcoord;
layout (location = 0) out vec4 color;
layout (location = 1) out int entity;

uniform samplerCube u_cubemap;

void main()
{
	color = texture(u_cubemap, v_texcoord);
    entity = -1;
} 