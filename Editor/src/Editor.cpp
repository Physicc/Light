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
		shader.reset(Light::Shader::create("../Light/shadersrc/texture.vs", "../Light/shadersrc/texture.fs"));

		vao.reset(Light::VertexArray::create());
		squareVao.reset(Light::VertexArray::create());

		float vertices[] = {
			0, 0, 0, 1.0, 0.0, 0.0, 1.0,
			0, 0.5, 0, 0.0, 1.0, 0.0, 1.0,
			0.5, 0, 0, 0.0, 0.0, 1.0, 1.0
		};

		float squareVertices[] = {
			-0.2, -0.2, 0, 0.3, 0.2, 0.8, 1.0,
			-0.2, 0.7, 0, 0.3, 0.2, 0.8, 1.0,
			0.7, -0.2, 0, 0.3, 0.2, 0.8, 1.0,
			0.7, 0.7, 0, 0.3, 0.2, 0.8, 1.0
		};

		vbo.reset(Light::VertexBuffer::create(vertices, sizeof(vertices)));
		squareVbo.reset(Light::VertexBuffer::create(squareVertices, sizeof(squareVertices)));

		Light::BufferLayout layout = {
			{ Light::ShaderDataType::Float3, "a_Position" },
			{ Light::ShaderDataType::Float4, "a_Color"}
		};

		vbo->setLayout(layout);
		squareVbo->setLayout(layout);

		unsigned int indices[3] = { 0, 1, 2};
		unsigned int squareIndices[6] = {0, 1, 2, 2, 1, 3};
		
		ibo.reset(Light::IndexBuffer::create(indices, sizeof(indices)/sizeof(unsigned int)));
		squareIbo.reset(Light::IndexBuffer::create(squareIndices, sizeof(squareIndices)/sizeof(unsigned int)));

		vao->addVertexBuffer(vbo);
		vao->setIndexBuffer(ibo);
		squareVao->addVertexBuffer(squareVbo);
		squareVao->setIndexBuffer(squareIbo);

	}
	~ExampleLayer() {}

	void onUpdate(Light::Timestep ts) override
	{

		cameraController.onUpdate(ts);
		cube->onUpdate(ts);

		Light::Renderer::beginScene(cameraController.getCamera(), glm::vec3(-1, 2, 1.5));
		
		Light::Renderer::submit(shader, squareVao);
		//Light::Renderer::submit(shader, vao);
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
	std::shared_ptr<Light::VertexArray> vao;
	std::shared_ptr<Light::VertexArray> squareVao;
	std::shared_ptr<Light::Shader> shader;
	std::shared_ptr<Light::VertexBuffer> vbo;
	std::shared_ptr<Light::VertexBuffer> squareVbo;
	std::shared_ptr<Light::IndexBuffer> ibo;
	std::shared_ptr<Light::IndexBuffer> squareIbo;
	Light::PerspectiveCameraController cameraController;
	std::shared_ptr<Cube> cube;
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