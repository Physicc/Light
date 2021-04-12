#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;

out vec4 v_color;

uniform mat4 u_viewProjectionMatrix;
uniform mat4 u_transform;

void main()
{
	gl_Position = u_viewProjectionMatrix *  u_transform * vec4(a_Position, 1.0);
	v_color = a_Color;
}