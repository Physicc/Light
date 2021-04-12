#include "Objects.hpp"

Cube::Cube()
	:	layout({
			{ Light::ShaderDataType::Float3, "a_Position" },
			{ Light::ShaderDataType::Float4, "a_Color" },
			{ Light::ShaderDataType::Float3, "a_Normal" }
		}),
		position(0,0,0),
		rotation(0,0,0),
		scale(0.5)
{
	shader.reset(Light::Shader::create("../Light/shadersrc/test.vs", "../Light/shadersrc/test.fs"));
	vao.reset(Light::VertexArray::create());

	float vertices[] = {
			//Front
			-0.5, -0.5, 0.5, 0.8, 0.8, 0.8, 1.0, 0, 0, 1.0,
			0.5, -0.5, 0.5, 0.8, 0.8, 0.8, 1.0, 0, 0, 1.0,
			0.5, 0.5, 0.5, 0.8, 0.8, 0.8, 1.0, 0, 0, 1.0,
			-0.5, 0.5, 0.5, 0.8, 0.8, 0.8, 1.0, 0, 0, 1.0,
			//Left
			-0.5, -0.5, 0.5, 0.8, 0.8, 0.8, 1.0, -1.0, 0, 0,
			-0.5, 0.5, 0.5, 0.8, 0.8, 0.8, 1.0, -1.0, 0, 0,
			-0.5, 0.5, -0.5, 0.8, 0.8, 0.8, 1.0, -1.0, 0, 0,
			-0.5, -0.5, -0.5, 0.8, 0.8, 0.8, 1.0, -1.0, 0, 0,
			//Right
			0.5, -0.5, 0.5, 0.8, 0.8, 0.8, 1.0, 1.0, 0, 0,
			0.5, -0.5, -0.5, 0.8, 0.8, 0.8, 1.0, 1.0, 0, 0,
			0.5, 0.5, -0.5, 0.8, 0.8, 0.8, 1.0, 1.0, 0, 0,
			0.5, 0.5, 0.5, 0.8, 0.8, 0.8, 1.0, 1.0, 0, 0,
			//Top
			-0.5, 0.5, 0.5, 0.8, 0.8, 0.8, 1.0, 0, 1.0, 0,
			0.5, 0.5, 0.5, 0.8, 0.8, 0.8, 1.0, 0, 1.0, 0,
			0.5, 0.5, -0.5, 0.8, 0.8, 0.8, 1.0, 0, 1.0, 0,
			-0.5, 0.5, -0.5, 0.8, 0.8, 0.8, 1.0, 0, 1.0, 0,
			//Bottom
			-0.5, -0.5, 0.5, 0.8, 0.8, 0.8, 1.0, 0, -1.0, 0,
			-0.5, -0.5, -0.5, 0.8, 0.8, 0.8, 1.0, 0, -1.0, 0,
			0.5, -0.5, -0.5, 0.8, 0.8, 0.8, 1.0, 0, -1.0, 0,
			0.5, -0.5, 0.5, 0.8, 0.8, 0.8, 1.0, 0, -1.0, 0,
			//Back
			-0.5, -0.5, -0.5, 0.8, 0.8, 0.8, 1.0, 0, 0, -1.0,
			-0.5, 0.5, -0.5, 0.8, 0.8, 0.8, 1.0, 0, 0, -1.0,
			0.5, 0.5, -0.5, 0.8, 0.8, 0.8, 1.0, 0, 0, -1.0,
			0.5, -0.5, -0.5, 0.8, 0.8, 0.8, 1.0, 0, 0, -1.0,
	};

	vbo.reset(Light::VertexBuffer::create(vertices, sizeof(vertices)));
	vbo->setLayout(layout);

	unsigned int indices[] = { 
		0, 1, 2, 2, 3, 0,
		4, 5, 6, 6, 7, 4,
		8, 9, 10, 10, 11, 8,
		12, 13, 14, 14, 15, 12,
		16, 17, 18, 18, 19, 16,
		20, 21, 22, 22, 23, 20
	};

	ibo.reset(Light::IndexBuffer::create(indices, sizeof(indices)/sizeof(unsigned int)));

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