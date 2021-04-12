#include "light.hpp"
#include "core/entrypoint.hpp"

#include "imgui.h"

#include "Objects.hpp"

class ExampleLayer : public Light::Layer
{
public:
	ExampleLayer(): Light::Layer("TestLayer"), 
			cameraController(45.0f, 1.6f/0.9f, 0.001f, 100.0f)
	{
		cube = std::make_shared<Cube>();
	}
	~ExampleLayer() {}

	void onUpdate(Light::Timestep ts) override
	{
		theta += 30 * ts.getSeconds();
		cameraController.onUpdate(ts);
		cube->onUpdate(ts);

		Light::Renderer::beginScene(cameraController.getCamera(), glm::vec3(2*glm::sin(glm::radians(theta)),2,2*glm::cos(glm::radians(theta))));

		cube->render();

		Light::Renderer::endScene();
	}

	void onEvent(Light::Event& e) override
	{
		cameraController.onEvent(e);

		Light::EventDispatcher dispatcher(e);
		dispatcher.Dispatch<Light::WindowResizeEvent>(BIND_EVENT_FN(onWindowResize));

		cube->onEvent(e);
	}

	void onImguiRender() override
	{
	}

	bool onWindowResize(Light::WindowResizeEvent& e)
	{
		cameraController.setAspectRatio(std::get<0>(e.getSize())/std::get<1>(e.getSize()));
		return false;
	}

private:
	Light::PerspectiveCameraController cameraController;
	std::shared_ptr<Cube> cube;
	float theta = 45;
};

class Editor : public Light::Application
{
public:
	Editor()
	{
		pushLayer(new ExampleLayer());
	}
	~Editor() {}
	
};

Light::Application* Light::createApplication()
{
	return new Editor();
}