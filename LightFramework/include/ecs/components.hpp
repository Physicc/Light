#ifndef __COMPONENTS_HPP__
#define __COMPONENTS_HPP__

#include <memory>
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include "light/rendering/shader.hpp"
#include "light/rendering/buffer.hpp"
#include "light/rendering/texture.hpp"
#include "light/rendering/vertexarray.hpp"
#include "light/rendering/camera.hpp"

namespace Light
{	
	struct TransformComponent
	{
		TransformComponent(glm::vec3 position = glm::vec3(0,0,0), 
			glm::vec3 rotation = glm::vec3(0,0,0), 
			glm::vec3 scale = glm::vec3(0.5))
		: position(position), rotation(rotation), scale(scale) {}
		inline glm::mat4 getTransform() const
		{
			return glm::translate(glm::mat4(1.0f), position)
				* glm::toMat4(glm::quat(rotation))
				* glm::scale(glm::mat4(1.0f), scale);
		}
		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 scale;
	};

	struct MeshRendererComponent
	{
		MeshRendererComponent(const char* path);
		inline void bind()
		{
			shader->bind();
		}
		inline void setUniformInt(const std::string& name, int value)
		{
			shader->setUniformInt(name, value);
		}
		std::shared_ptr<Light::Shader> shader;
	};

	struct MeshComponent
	{
		MeshComponent(std::shared_ptr<Light::VertexBuffer> vertexBuffer,
			std::shared_ptr<Light::IndexBuffer> indexBuffer);
		std::shared_ptr<Light::VertexArray> mesh;
	};

	struct LightComponent
	{
		LightComponent() : m_light_color({1.0, 1.0, 1.0}) {}
		LightComponent(glm::vec3 light_color) : m_light_color(light_color) {}
		glm::vec3 m_light_color;
	};

	struct CameraComponent
	{
		CameraComponent() = default;
		CameraComponent(glm::mat4 projectionMatrix) : m_camera(projectionMatrix) {}
		Camera m_camera;
	};
}

#endif // __COMPONENTS_HPP__
