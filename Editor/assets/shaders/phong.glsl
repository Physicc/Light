#type vertex
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

#type fragment
#version 330 core

in vec3 v_normal;
in vec4 v_color;
in vec3 v_worldPos;
out vec4 color;

uniform vec3 u_lightPos;

void main()
{
	vec3 lightCol = vec3(1.0, 1.0, 1.0);

	vec3 norm = normalize(v_normal);
	vec3 lightDir = normalize(u_lightPos - v_worldPos);

	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff*lightCol;

	vec3 ambient = vec3(v_color) * 0.3;

	color = vec4(ambient + diffuse, 1.0) * v_color;
}
