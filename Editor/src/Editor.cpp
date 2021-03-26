#include "light.hpp"

#include "imgui.h"

//Temp
#include "glad/glad.h"

class ExampleLayer : public Light::Layer
{
public:
	ExampleLayer(): Light::Layer("TestLayer")
	{
		shader = Light::Shader::create("../Light/shadersrc/test.vs", "../Light/shadersrc/test.fs");

		vao = Light::VertexArray::create();

		float vertices[] = {
			0, 0, 0, 1.0, 0.0, 0.0, 1.0,
			0, 0.5, 0, 0.0, 1.0, 0.0, 1.0,
			0.5, 0, 0, 0.0, 0.0, 1.0, 1.0
		};

		vbo.reset(Light::VertexBuffer::create(vertices, sizeof(vertices)));

		Light::BufferLayout layout = {
			{ Light::ShaderDataType::Float3, "a_Position" },
			{ Light::ShaderDataType::Float4, "a_Color"}
		};

		vbo->setLayout(layout);

		unsigned int indices[3] = { 0, 1, 2};
		
		ibo.reset(Light::IndexBuffer::create(indices, sizeof(indices)/sizeof(unsigned int)));

		vao->addVertexBuffer(vbo);
		vao->setIndexBuffer(ibo);
	}
	~ExampleLayer() {}

	void onUpdate() override
	{
		shader->bind();

		vao->bind();
		glDrawElements(GL_TRIANGLES, vao->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, nullptr);

		shader->unbind();
	}

	void onEvent(Light::Event& e) override
	{

	}

	void onImguiRender() override
	{
	}

private:
	Light::VertexArray* vao;
	Light::Shader* shader;
	std::shared_ptr<Light::VertexBuffer> vbo;
	std::shared_ptr<Light::IndexBuffer> ibo;
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