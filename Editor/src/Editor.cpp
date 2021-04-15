#include "light.hpp"
#include "core/entrypoint.hpp"

#include "imgui.h"

#include "Objects.hpp"
//Temp
#include "platform/opengl/openglshader.hpp"

class ExampleLayer : public Light::Layer
{
public:
	ExampleLayer(): Light::Layer("TestLayer"), 
			camera(glm::radians(45.0f), 1.6f/0.9f, 0.001f, 100.0f),
			lightPos(-1,2,1.5),
			cameraPos(-0.754, 0.651, 1.758),
			floor(glm::vec3(0,-1,0), glm::vec3(0), glm::vec3(2,0.1,2))
	{

	}
	~ExampleLayer() {}

	void onUpdate(Light::Timestep ts) override
	{
		camera.setPosition(cameraPos);
		camera.onUpdate(ts);
		cube.onUpdate(ts);

		Light::Renderer::beginScene(camera, lightPos);
		
		skybox.render();
		cube.render();
		floor.render();

		Light::Renderer::endScene();
	}

	void onEvent(Light::Event& e) override
	{
		camera.onEvent(e);
		cameraPos = camera.getPosition();

		cube.onEvent(e);
	}

	void onImguiRender() override
	{
		cameraPos = camera.getPosition();
		ImGui::SliderFloat3("Camera Position", &(cameraPos.x), -5.0f, 5.0f);
		ImGui::SliderFloat3("Light Position", &(lightPos.x), -5.0f, 5.0f);
	}

private:
	Light::EditorCamera camera;
	Cube cube;
	Cube floor;
	Skybox skybox;
	glm::vec3 cameraPos;
	glm::vec3 lightPos;

	std::shared_ptr<Light::VertexArray> vao;
	std::shared_ptr<Light::VertexBuffer> vbo;
	std::shared_ptr<Light::IndexBuffer> ibo;
	std::shared_ptr<Light::Shader> shader;

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