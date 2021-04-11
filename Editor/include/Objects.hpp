#ifndef __OBJECTS_H__
#define __OBJECTS_H__

#include "light.hpp"

class Cube
{
public:
	Cube();

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
	float scale;
};



#endif // __OBJECTS_H__