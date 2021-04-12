#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec3 a_Normal;
out vec4 v_color;
out vec3 v_normal;
out vec3 v_worldPos;

uniform mat4 u_viewProjectionMatrix;
uniform mat4 u_transform;
uniform mat3 u_normal;

void main()
{
	gl_Position = u_viewProjectionMatrix *  u_transform * vec4(a_Position, 1.0);
	v_color = a_Color;
	v_normal = u_normal * a_Normal;
	v_worldPos = vec3(u_transform * vec4(a_Position, 1.0));
}