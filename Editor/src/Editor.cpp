#include "light.hpp"

class Editor : public Light::Application
{
public:
	Editor() {}
	~Editor() {}
	
};

Light::Application* Light::createApplication()
{
	return new Editor();
}