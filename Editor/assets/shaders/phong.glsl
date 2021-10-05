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
layout(location = 0) out vec4 color;
layout(location = 1) out int entity;

struct PointLight
{
	vec4 position;
	vec4 color;
};

uniform PointLight u_pointLights[4];
uniform int u_numPointLights;
uniform int u_id;
uniform int u_selectionId;

vec4 pointLightCalculate(PointLight light, vec3 norm)
{
	float distance = length(light.position.xyz - v_worldPos);
	vec3 lightDir = (light.position.xyz - v_worldPos) / distance;

	float attentuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * distance * distance);

	float diff = max(dot(norm, lightDir), 0.0);
	vec4 diffuse = diff * light.color;

	return diffuse * attentuation;
}

void main()
{
	vec3 norm = normalize(v_normal);
	color = vec4(0.3, 0.3, 0.3, 1.0);
	// for (int i = 0; i < u_numPointLights; i++)
	// {
	// 	color += vec4(pointLightCalculate(u_pointLights[i]), 0.0);
	// }
	color += pointLightCalculate(u_pointLights[0], norm);
	color += pointLightCalculate(u_pointLights[1], norm);
	color += pointLightCalculate(u_pointLights[2], norm);
	color += pointLightCalculate(u_pointLights[3], norm);
	color.a = 1.0;
	color *= v_color;
	entity = u_id;
}
