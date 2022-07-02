#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec3 a_Normal;
layout(location = 3) in vec2 a_TexCoord;

out vec3 v_worldPos;
out vec3 v_normal;
out vec2 v_texCoord;

uniform mat4 u_viewProjectionMatrix;
uniform mat4 u_transform; //model 


void main()
{
	gl_Position = u_viewProjectionMatrix *  u_transform * vec4(a_Position, 1.0);
	v_worldPos = vec3(u_transform * vec4(a_Position, 1.0));
	v_normal = normalize(mat3(transpose(inverse(u_transform))) * a_Normal);
	v_texCoord = a_TexCoord;
}
