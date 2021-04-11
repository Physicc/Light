#include "Objects.hpp"

Cube::Cube()
	:	layout({
			{ Light::ShaderDataType::Float3, "a_Position" },
			{ Light::ShaderDataType::Float4, "a_Color"}
		}),
		position(0,0,0),
		rotation(0,0,0),
		scale(0.5)
{
	shader.reset(Light::Shader::create("../Light/shadersrc/test.vs", "../Light/shadersrc/test.fs"));
	vao.reset(Light::VertexArray::create());

	float vertices[] = {
			0, 0, 0, 1.0, 0.0, 0.0, 1.0,
			0, 1.0, 0, 0.0, 1.0, 0.0, 1.0,
			1.0, 0, 0, 0.0, 0.0, 1.0, 1.0
	};

	vbo.reset(Light::VertexBuffer::create(vertices, sizeof(vertices)));
	vbo->setLayout(layout);

	unsigned int indices[3] = { 0, 1, 2};

	ibo.reset(Light::IndexBuffer::create(indices, 6));

	vao->addVertexBuffer(vbo);
	vao->setIndexBuffer(ibo);
}

void Cube::onUpdate(Light::Timestep ts) 
{
	if(Light::Input::isKeyPressed(LIGHT_KEY_A))
	{
		position.x -= 1.0 * ts.getSeconds();
	}
	else if(Light::Input::isKeyPressed(LIGHT_KEY_D))
	{
		position.x += 1.0 * ts.getSeconds();
	}
	else if(Light::Input::isKeyPressed(LIGHT_KEY_W))
	{
		position.z += 1.0 * ts.getSeconds();
	}
	else if(Light::Input::isKeyPressed(LIGHT_KEY_S))
	{
		position.z -= 1.0 * ts.getSeconds();
	}
	else if(Light::Input::isKeyPressed(LIGHT_KEY_1))
	{
		rotation.x += 5.0 * ts.getSeconds();
	}
	else if(Light::Input::isKeyPressed(LIGHT_KEY_2))
	{
		rotation.x -= 5.0 * ts.getSeconds();
	}
	else if(Light::Input::isKeyPressed(LIGHT_KEY_3))
	{
		scale += 0.2 * ts.getSeconds();
	}
	else if(Light::Input::isKeyPressed(LIGHT_KEY_4))
	{
		scale -= 0.2 * ts.getSeconds();
	}

	transform = glm::translate(glm::mat4(1.0f), position);
	transform = glm::scale(transform, glm::vec3(scale));
	transform = glm::rotate(transform, glm::radians(rotation.x), glm::vec3(1.0, 0.0, 0.0));
	transform = glm::rotate(transform, glm::radians(rotation.y), glm::vec3(0.0, 1.0, 0.0));
	transform = glm::rotate(transform, glm::radians(rotation.z), glm::vec3(0.0, 0.0, 1.0));
}

void Cube::onEvent(Light::Event& e) 
{
	
}

void Cube::render()
{
	Light::Renderer::submit(shader, vao, transform);
}