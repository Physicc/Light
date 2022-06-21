#version 330 core

in vec3 v_worldPos;
in vec3 v_normal;
in vec2 v_texCoord;

layout(location = 0) out vec4 color;
layout(location = 1) out int entity;

struct DirectionalLight
{
	sampler2D depthMap;
	vec4 emission_color;
	vec3 direction;
	mat4 lightSpaceMatrix;
};

struct PointLight
{
	samplerCube depthCubemap;
	vec4 emission_color;
	vec3 position;
	float far_plane;
};

struct SpotLight
{
	samplerCube depthCubemap;
	vec4 emission_color;
	vec3 position;
	vec3 direction;
	float far_plane;
	float innerCutoff;
	float outerCutoff;
};


float calculateShadow(DirectionalLight light)
{
	vec4 fragPosLightSpace = light.lightSpaceMatrix * vec4(v_worldPos, 1.0);
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
 
    projCoords = projCoords * 0.5 + 0.5;
    float closestDepth = texture(light.depthMap, projCoords.xy).r; 

    float currentDepth = projCoords.z;

    float bias = 0.005;

	float shadow = (currentDepth - bias > closestDepth ) ? 1.0 : 0.0; 
  
    if(projCoords.z > 1.0)
        shadow = 0.0;
        
    return shadow;
}

float calculateShadow(PointLight light)
{
    // get vector between fragment position and light position
    vec3 fragToLight = v_worldPos - light.position;
    // use the light to fragment vector to sample from the depth map    
    float closestDepth = texture(light.depthCubemap, fragToLight).r;
    // it is currently in linear range between [0,1]. Re-transform back to original value
    closestDepth *= light.far_plane;
    // now get current linear depth as the length between the fragment and light position
    float currentDepth = length(fragToLight);
    // now test for shadows
    float bias = 0.05; 
    float shadow = currentDepth -  bias > closestDepth ? 1.0 : 0.0;

    return shadow;
}  

float calculateShadow(SpotLight light)
{
    // get vector between fragment position and light position
    vec3 fragToLight = v_worldPos - light.position;
    // use the light to fragment vector to sample from the depth map    
    float closestDepth = texture(light.depthCubemap, fragToLight).r;
    // it is currently in linear range between [0,1]. Re-transform back to original value
    closestDepth *= light.far_plane;
    // now get current linear depth as the length between the fragment and light position
    float currentDepth = length(fragToLight);
    // now test for shadows
    float bias = 0.05; 
    float shadow = currentDepth -  bias > closestDepth ? 1.0 : 0.0;

    return shadow;
}

vec4 calculateShading(DirectionalLight light, vec3 viewDir)
{	
	vec3 fragToLightDir = -normalize(light.direction);
	float diff = max(dot(v_normal, fragToLightDir), 0.0);
	vec4 diffuse = diff * light.emission_color;


	//specular
	vec3 halfwayDir = normalize(fragToLightDir - viewDir);
	//TODO: make sure it is facing light
	float spec = pow(max(dot(halfwayDir, v_normal), 0.0), 32);
	vec4 specular = spec * light.emission_color;
	float shadow = calculateShadow(light);
	vec4 result = (1.0f - shadow)*(diffuse + specular);
//	vec4 result = (1.0f - shadow) * vec4(1, 1, 1, 1);

	return result;
}

vec4 calculateShading(PointLight light, vec3 viewDir)
{
	float distance = length(light.position - v_worldPos);
	vec3 lightDir = (light.position - v_worldPos) / distance;

	float attentuation = clamp(1 - (distance * distance)/(light.far_plane * light.far_plane), 0.0 , 1.0);

	float diff = max(dot(v_normal, lightDir), 0.0);
	vec4 diffuse = diff * light.emission_color;

	//specular
	vec3 halfwayDir = normalize(lightDir - viewDir);
	float spec = pow(max(dot(v_normal, halfwayDir), 0.0), 64);
	vec4 specular = spec * light.emission_color;
	diffuse = diffuse * attentuation;
	specular = specular * attentuation;
	float shadow = calculateShadow(light);
	vec4 result = (1.0f - shadow)*(diffuse + specular);
	return result;
//	return vec4(1.0 , 0.0 , 0.0 , 0.0);
}

vec4 calculateShading(SpotLight light, vec3 viewDir)
{
 	float distance = length(light.position - v_worldPos);
	vec3 lightDir = normalize(light.position - v_worldPos);
	vec4 result;
//
//	
	float attentuation = clamp(1 - distance/light.far_plane, 0.0 , 1.0);
	float theta = dot(lightDir, -light.direction);
	if (theta > light.outerCutoff)
	{	
		float intensity = clamp((theta - light.outerCutoff) / (light.innerCutoff - light.outerCutoff), 0.0, 1.0);
		float diff = max(dot(v_normal, lightDir), 0.0);
		vec4 diffuse = diff * light.emission_color;
		vec3 halfwayDir = normalize(lightDir - viewDir);
		float spec = pow(max(dot(halfwayDir, v_normal), 0.0), 64);
		vec4 specular = spec * light.emission_color;
		float shadow = calculateShadow(light);
//		diffuse = diffuse * intensity * attentuation;
//		specular = specular * intensity * attentuation;
		result = (diffuse + specular) * attentuation * intensity * (1.0f - shadow);
	}
	else
	{
		result = vec4(0.0 , 0.0 , 0.0 , 1.0);
	}
	return result;
//	return vec4(lightDir * attentuation, 1.0);
}

uniform int u_id;
uniform int u_n_dLights;
uniform int u_n_pLights;
uniform int u_n_sLights;
uniform vec3 cameraPos;
//uniform sampler2D diffuseTexture;
uniform DirectionalLight u_dirLights[4];
uniform PointLight u_pointLights[4];
uniform SpotLight u_spotLights[4];


void main()
{	
//	color = vec4(texture(diffuseTexture, v_texCoord));
	color = vec4(1.0, 1.0, 1.0, 1.0);
	vec3 viewDir = normalize(v_worldPos - cameraPos);

	vec4 lighting = vec4(0.0, 0.0, 0.0, 0.0);
	for(int i=0; i<u_n_dLights; i++)
		lighting += calculateShading(u_dirLights[i], viewDir);
	for(int i=0; i<u_n_pLights; i++)
		lighting += calculateShading(u_pointLights[i], viewDir);
	for(int i=0; i<u_n_sLights; i++)
		lighting += calculateShading(u_spotLights[i], viewDir);

	color *= lighting;
	color.a = 1.0;
	entity = u_id;
}
