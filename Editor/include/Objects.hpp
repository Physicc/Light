#ifndef __OBJECTS_H__
#define __OBJECTS_H__

#include "light.hpp"

class Cube
{
public:
	Cube(glm::vec3 position = glm::vec3(0,0,0), 
		glm::vec3 rotation = glm::vec3(0,0,0), 
		glm::vec3 scale = glm::vec3(0.5));

	void render();

	void onUpdate(Light::Timestep ts);
	void onEvent(Light::Event& e);

	inline glm::mat4 getTransform() { return transform; }

private:
	Light::BufferLayout layout;
	std::shared_ptr<Light::VertexArray> vao;
	std::shared_ptr<Light::VertexBuffer> vbo;
	std::shared_ptr<Light::IndexBuffer> ibo;
	std::shared_ptr<Light::Shader> shader;

	glm::mat4 transform;
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	void updateTransform();
};

class Skybox
{
public:
	Skybox();

	void render();

private:
	Light::BufferLayout layout;
	std::shared_ptr<Light::VertexArray> vao;
	std::shared_ptr<Light::VertexBuffer> vbo;
	std::shared_ptr<Light::IndexBuffer> ibo;
	std::shared_ptr<Light::Shader> shader;
	std::shared_ptr<Light::Cubemap> cubemap;
};




#endif // __OBJECTS_H__