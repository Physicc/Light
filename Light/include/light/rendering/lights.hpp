#ifndef __LIGHTS_H__
#define __LIGHTS_H__

#include "glm/glm.hpp"
#include <glm/gtx/quaternion.hpp>

#include <string>
#include <vector>

namespace Light
{

	enum class LightType
	{
		Directional = 0,
		Point = 1,
		Spot = 2,
		NumLightTypes
	};

	extern std::string LightTypeStrings[(int)LightType::NumLightTypes];

// TODO convert these lights to classes
	struct PointLight
	{
		glm::vec3 position;
		glm::vec3 color;
		float range;

		std::vector<glm::mat4> getSpaceMatrices() const
		{
			glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 25.0f);
			std::vector<glm::mat4> shadowTransforms;
			shadowTransforms.push_back(shadowProj * 
							glm::lookAt(position, position + glm::vec3( 1.0, 0.0, 0.0), glm::vec3(0.0,-1.0, 0.0)));
			shadowTransforms.push_back(shadowProj * 
							glm::lookAt(position, position + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0,-1.0, 0.0)));
			shadowTransforms.push_back(shadowProj * 
							glm::lookAt(position, position + glm::vec3( 0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
			shadowTransforms.push_back(shadowProj * 
							glm::lookAt(position, position + glm::vec3( 0.0,-1.0, 0.0), glm::vec3(0.0, 0.0,-1.0)));
			shadowTransforms.push_back(shadowProj * 
							glm::lookAt(position, position + glm::vec3( 0.0, 0.0, 1.0), glm::vec3(0.0,-1.0, 0.0)));
			shadowTransforms.push_back(shadowProj * 
							glm::lookAt(position, position + glm::vec3( 0.0, 0.0,-1.0), glm::vec3(0.0,-1.0, 0.0)));
			return shadowTransforms;
		}
	};

	struct SpotLight
	{
		glm::vec3 position;
		glm::vec3 color;
		glm::vec3 direction;
		float innerCutoff;
		float outerCutoff;
		float range;
	};

	struct DirectionalLight
	{
		glm::vec3 position;
		glm::vec3 direction;
		glm::vec3 color;
		// glm::mat4 lightSpaceMatrix;

		inline glm::mat4 getViewMatrix() const
		{
			return glm::lookAt(-10.0f * direction, glm::vec3(0.0f), glm::vec3(0.0,1.0,0.0));
		}
		inline glm::mat4 getProjectionMatrix() const
		{
			return glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.0f, 20.0f);
		}
		inline glm::mat4 getSpaceMatrix() const
		{
			return  getProjectionMatrix() * getViewMatrix();
		}
	};
}

#endif // __LIGHTS_H__
