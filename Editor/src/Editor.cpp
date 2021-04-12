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
			cameraController(45.0f, 1.6f/0.9f, 0.001f, 100.0f)
	{
		cube = std::make_shared<Cube>();
		shader.reset(Light::Shader::create("../Light/assets/shaders/texture.vs", "../Light/assets/shaders/texture.fs"));

		squareVao.reset(Light::VertexArray::create());

		float squareVertices[] = {
			-0.2, -0.2, 0, 0, 0,
			-0.2, 0.7, 0, 0, 1,
			0.7, -0.2, 0, 1, 0,
			0.7, 0.7, 0, 1, 1,
		};

		squareVbo.reset(Light::VertexBuffer::create(squareVertices, sizeof(squareVertices)));

		Light::BufferLayout layout = {
			{ Light::ShaderDataType::Float3, "a_Position" },
			{ Light::ShaderDataType::Float2, "a_TexCoord"}
		};

		squareVbo->setLayout(layout);

		unsigned int squareIndices[6] = {0, 1, 2, 2, 1, 3};
		
		squareIbo.reset(Light::IndexBuffer::create(squareIndices, sizeof(squareIndices)/sizeof(unsigned int)));

		squareVao->addVertexBuffer(squareVbo);
		squareVao->setIndexBuffer(squareIbo);

		texture.reset(Light::Texture2D::create("../Light/assets/textures/Checkerboard.png"));
		std::dynamic_pointer_cast<Light::OpenGLShader>(shader)->bind();
		std::dynamic_pointer_cast<Light::OpenGLShader>(shader)->setUniformInt("u_texture", 0);

	}
	~ExampleLayer() {}

	void onUpdate(Light::Timestep ts) override
	{

		cameraController.onUpdate(ts);
		cube->onUpdate(ts);

		Light::Renderer::beginScene(cameraController.getCamera(), glm::vec3(-1, 2, 1.5));
		
		skybox.render();
		texture->bind();
		Light::Renderer::submit(shader, squareVao);
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
	std::shared_ptr<Light::VertexArray> squareVao;
	std::shared_ptr<Light::Shader> shader;
	std::shared_ptr<Light::VertexBuffer> squareVbo;
	std::shared_ptr<Light::IndexBuffer> squareIbo;
	Light::PerspectiveCameraController cameraController;
	std::shared_ptr<Cube> cube;
	std::shared_ptr<Light::Texture2D> texture; 
	Skybox skybox;

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