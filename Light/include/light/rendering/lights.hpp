#ifndef __LIGHTS_H__
#define __LIGHTS_H__

#include "glm/glm.hpp"
#include <glm/gtx/quaternion.hpp>

#include <string>

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

	struct PointLight
	{
		glm::vec3 position;
		glm::vec3 color;
		float range;
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
