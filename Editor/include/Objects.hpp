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
	inline glm::mat4 getTransform() { return m_transform; }

private:
	Light::BufferLayout m_layout;
	std::shared_ptr<Light::VertexArray> m_vao;
	std::shared_ptr<Light::VertexBuffer> m_vbo;
	std::shared_ptr<Light::IndexBuffer> m_ibo;
	std::shared_ptr<Light::Shader> m_shader;
	glm::mat4 m_transform;
	glm::vec3 m_position;
	glm::vec3 m_rotation;
	glm::vec3 m_scale;

	void updateTransform();
};

class Skybox
{
public:
	Skybox();
	void render();

private:
	Light::BufferLayout m_layout;
	std::shared_ptr<Light::VertexArray> m_vao;
	std::shared_ptr<Light::VertexBuffer> m_vbo;
	std::shared_ptr<Light::IndexBuffer> m_ibo;
	std::shared_ptr<Light::Shader> m_shader;
	std::shared_ptr<Light::Cubemap> m_cubemap;
};




#endif // __OBJECTS_H__