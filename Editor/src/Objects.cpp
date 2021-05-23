#include "Objects.hpp"

Cube::Cube(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
	:	layout({
			{ Light::ShaderDataType::Float3, "a_Position" },
			{ Light::ShaderDataType::Float4, "a_Color" },
			{ Light::ShaderDataType::Float3, "a_Normal" }
		}),
		position(position),
		rotation(rotation),
		scale(scale)
{
	shader = Light::Shader::create("../Light/assets/shaders/phong.glsl");
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

	updateTransform();
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
		position.z -= 1.0 * ts.getSeconds();
	}
	else if(Light::Input::isKeyPressed(LIGHT_KEY_S))
	{
		position.z += 1.0 * ts.getSeconds();
	}
	else if(Light::Input::isKeyPressed(LIGHT_KEY_1))
	{
		rotation.x += 20.0 * ts.getSeconds();
	}
	else if(Light::Input::isKeyPressed(LIGHT_KEY_2))
	{
		rotation.x -= 20.0 * ts.getSeconds();
	}
	else if(Light::Input::isKeyPressed(LIGHT_KEY_3))
	{
		scale += 0.2 * ts.getSeconds();
	}
	else if(Light::Input::isKeyPressed(LIGHT_KEY_4))
	{
		scale -= 0.2 * ts.getSeconds();
	}

	updateTransform();
}

void Cube::onEvent(Light::Event& e) 
{
	
}

void Cube::render()
{
	Light::Renderer::submit(shader, vao, transform);
}

void Cube::updateTransform()
{
	transform = glm::translate(glm::mat4(1.0f), position);
	transform = glm::scale(transform, scale);
	transform = glm::rotate(transform, glm::radians(rotation.x), glm::vec3(1.0, 0.0, 0.0));
	transform = glm::rotate(transform, glm::radians(rotation.y), glm::vec3(0.0, 1.0, 0.0));
	transform = glm::rotate(transform, glm::radians(rotation.z), glm::vec3(0.0, 0.0, 1.0));
}



Skybox::Skybox()
	: layout({
			{ Light::ShaderDataType::Float3, "a_Position" }
		})
{
	vao.reset(Light::VertexArray::create());

	float vertices[] = {
			//Front
			-1.0, -1.0, 1.0,
			1.0, -1.0, 1.0,
			1.0, 1.0, 1.0,
			-1.0, 1.0, 1.0,
			//Left
			-1.0, -1.0, 1.0,
			-1.0, 1.0, 1.0,
			-1.0, 1.0, -1.0,
			-1.0, -1.0, -1.0,
			//Right
			1.0, -1.0, 1.0,
			1.0, -1.0, -1.0,
			1.0, 1.0, -1.0,
			1.0, 1.0, 1.0,
			//Top
			-1.0, 1.0, 1.0,
			1.0, 1.0, 1.0,
			1.0, 1.0, -1.0,
			-1.0, 1.0, -1.0,
			//Bottom
			-1.0, -1.0, 1.0,
			-1.0, -1.0, -1.0,
			1.0, -1.0, -1.0,
			1.0, -1.0, 1.0,
			//Back
			-1.0, -1.0, -1.0,
			-1.0, 1.0, -1.0,
			1.0, 1.0, -1.0,
			1.0, -1.0, -1.0,
	};

	vbo.reset(Light::VertexBuffer::create(vertices, sizeof(vertices)));
	vbo->setLayout(layout);

	unsigned int indices[] = { 
		0, 2, 1, 3, 2, 0,
		4, 6, 5, 7, 6, 4,
		8, 10, 9, 11, 10, 8,
		12, 14, 13, 15, 14, 12,
		16, 18, 17, 19, 18, 16,
		20, 22, 21, 23, 22, 20
	};

	ibo.reset(Light::IndexBuffer::create(indices, sizeof(indices)/sizeof(unsigned int)));

	vao->addVertexBuffer(vbo);
	vao->setIndexBuffer(ibo);

	cubemap.reset(Light::Cubemap::create("../Light/assets/cubemap"));
	shader = Light::Shader::create("../Light/assets/shaders/skybox.glsl");
	shader->bind();
	shader->setUniformInt("u_cubemap", 0);
}

void Skybox::render() 
{
	cubemap->bind();
	Light::Renderer::submitSkybox(shader, vao);
}