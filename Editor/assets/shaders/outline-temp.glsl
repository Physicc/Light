#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;

uniform mat4 u_viewProjectionMatrix;
uniform mat4 u_transform;

void main()
{
	gl_Position = u_viewProjectionMatrix *  u_transform * vec4(a_Position, 1.0);
}

#type fragment
#version 330 core

in vec3 v_normal;
in vec4 v_color;
in vec3 v_worldPos;

layout(location = 0) out vec4 selection;

void main()
{
	selection = vec4(1.0,1.0,1.0,1.0);
}
