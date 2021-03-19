#include "light.hpp"

class Sandbox : public Light::Application
{
public:
	Sandbox() {}
	~Sandbox() {}
	
};

Light::Application* Light::createApplication()
{
	return new Sandbox();
}