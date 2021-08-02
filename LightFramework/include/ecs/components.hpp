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
#include "core/uuid.hpp"

namespace Light
{

	struct Component
	{
		std::string uuid;

		Component() : uuid(newUUID()) {}
	};

	struct TagComponent : public Component
	{
		std::string tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag) : tag(tag) {}
	};
	struct TransformComponent : public Component
	{
		TransformComponent(glm::vec3 position = glm::vec3(0,0,0), 
			glm::vec3 rotation = glm::vec3(0,0,0), 
			glm::vec3 scale = glm::vec3(0.5))
		: position(position), rotation(rotation), scale(scale) {}
		inline glm::mat4 getTransform() const
		{
			return glm::translate(glm::mat4(1.0f), position)
				* glm::toMat4(glm::quat(rotation))
				* glm::scale(glm::mat4(1.0f), glm::abs(scale));
		}
		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 scale;
	};

	struct MeshRendererComponent : public Component
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

	struct MeshComponent : public Component
	{
		MeshComponent(std::shared_ptr<Light::VertexBuffer> vertexBuffer,
			std::shared_ptr<Light::IndexBuffer> indexBuffer);
		std::shared_ptr<Light::VertexArray> mesh;
	};

	struct LightComponent : public Component
	{
		LightComponent() : m_lightColor({1.0, 1.0, 1.0}) {}
		LightComponent(glm::vec3 lightColor) : m_lightColor(lightColor) {}
		glm::vec3 m_lightColor;
	};

	struct CameraComponent : public Component
	{
		CameraComponent() = default;
		CameraComponent(glm::mat4 projectionMatrix) : m_camera(projectionMatrix) {}
		Camera m_camera;
	};
}

#endif // __COMPONENTS_HPP__
