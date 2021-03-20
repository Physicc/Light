#include "light.hpp"

class ExampleLayer : public Light::Layer
{
public:
	ExampleLayer(): Light::Layer("TestLayer") {}
	~ExampleLayer() {}

	void onUpdate() override
	{

	}

	void onEvent(Light::Event& e) override
	{

	}
};

class Editor : public Light::Application
{
public:
	Editor()
	{
		pushLayer(new ExampleLayer());
		pushOverlay(new Light::ImguiLayer("imgui"));
	}
	~Editor() {}
	
};

Light::Application* Light::createApplication()
{
	return new Editor();
}