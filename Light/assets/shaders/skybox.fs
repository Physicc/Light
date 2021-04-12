#version 330 core

in vec3 v_texcoord;
out vec4 color;

uniform samplerCube u_cubemap;

void main()
{
	color = texture(u_cubemap, v_texcoord);
} 