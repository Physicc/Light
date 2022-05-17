#include "light/rendering/lights.hpp"


namespace Light
{
	std::string LightTypeStrings[(int)LightType::NumLightTypes] = {
		"Directional",
		"Point",
		"Spot"
	};
}
