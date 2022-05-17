#ifndef __LIGHTS_H__
#define __LIGHTS_H__

#include "glm/glm.hpp"
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
	};
}

#endif // __LIGHTS_H__
