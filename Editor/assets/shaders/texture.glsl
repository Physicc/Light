#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;

out vec2 v_texcoord;

uniform mat4 u_viewProjectionMatrix;
uniform mat4 u_transform;

void main()
{
	gl_Position = u_viewProjectionMatrix *  u_transform * vec4(a_Position, 1.0);
	v_texcoord = a_TexCoord;
}

#type fragment
#version 330 core

in vec2 v_texcoord;
out vec4 color;

uniform sampler2D u_texture;

void main()
{
	color = texture(u_texture, v_texcoord);	
}