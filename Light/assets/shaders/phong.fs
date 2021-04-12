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