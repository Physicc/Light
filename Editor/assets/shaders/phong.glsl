#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec3 a_Normal;
layout(location = 3) in vec2 a_TexCoord;

out vec3 v_worldPos;
out vec2 v_texCoord;
out vec4 fragPosLightSpace;

uniform mat4 u_viewProjectionMatrix;
uniform mat4 lightSpaceMatrix;
uniform mat4 u_transform; //model 


void main()
{
	gl_Position = u_viewProjectionMatrix *  u_transform * vec4(a_Position, 1.0);
	v_worldPos = vec3(u_transform * vec4(a_Position, 1.0));
	fragPosLightSpace = lightSpaceMatrix * vec4(v_worldPos , 1.0);
	v_texCoord = a_TexCoord;
}

#type fragment
#version 330 core

in vec3 v_worldPos;
in vec2 v_texCoord;
in vec4 fragPosLightSpace;

layout(location = 0) out vec4 color;
layout(location = 1) out int entity;

uniform sampler2D depthMap;
uniform int u_id;

float shadowCalculate()
{

    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
 
    projCoords = projCoords * 0.5 + 0.5;
    float closestDepth = texture(depthMap, projCoords.xy).r; 

    float currentDepth = projCoords.z;

    float bias = 0.005;

	float shadow = (currentDepth - bias > closestDepth ) ? 1.0 : 0.0; 
  
    if(projCoords.z > 1.0)
        shadow = 0.0;
        
    return shadow;
}



void main()
{	
	color = vec4(texture(depthMap, v_texCoord).r);
	float shadow = shadowCalculate();
	color *= (1.0 - shadow);
	color.a = 1.0;
	entity = u_id;
}
