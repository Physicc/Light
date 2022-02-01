#ifndef __LIGHTS_H__
#define __LIGHTS_H__

#include "glm/glm.hpp"

namespace Light
{
	struct PointLight
	{
		glm::vec3 position;
		glm::vec3 color;
	};
}

#endif // __LIGHTS_H__