#ifndef __COMPONENTS_HPP__
#define __COMPONENTS_HPP__

#include <memory>
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include "light/rendering/shader.hpp"
#include "light/rendering/buffer.hpp"
#include "light/rendering/mesh.hpp"
#include "light/rendering/texture.hpp"
#include "light/rendering/vertexarray.hpp"
#include "light/rendering/camera.hpp"
#include "light/rendering/lights.hpp"
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
		TagComponent& operator=(const TagComponent&) = default;
		TagComponent(const std::string& tag) : tag(tag) {}
	};
	struct TransformComponent : public Component
	{
		TransformComponent(glm::vec3 position = glm::vec3(0, 0, 0),
						   glm::vec3 rotation = glm::vec3(0, 0, 0),
						   glm::vec3 scale = glm::vec3(0.5))
			: position(position), rotation(rotation), scale(scale) {}
		inline glm::mat4 getTransform() const
		{
			return glm::translate(glm::mat4(1.0f), position) * glm::toMat4(glm::quat(rotation)) * glm::scale(glm::mat4(1.0f), glm::abs(scale));
		}
		inline glm::mat4 getViewMatrix() const
		{
			return glm::lookAt(position, glm::vec3(0.0f), glm::vec3(0.0,1.0,0.0));
		}
		inline glm::mat4 getModel() const
		{
			glm::mat4 model = glm::mat4(1.0);
			model = glm::translate(model, position);
			model = glm::scale(model, scale);
			return model;
		}		
		inline glm::mat4 getProjectionMatrix() const
		{
			return glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 7.5f);
		}
		inline glm::mat4 getSpaceMatrix() const
		{
			return  getProjectionMatrix() * getViewMatrix();
		}
		inline glm::mat4 getOrientationMatrix() const
		{
			return glm::toMat4(glm::quat(rotation));
		}

		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 scale;
	};

	struct MeshRendererComponent : public Component
	{
		MeshRendererComponent(const char* path);
		MeshRendererComponent(const char* vertexPath, const char* fragmentPath);
		MeshRendererComponent(const char* vertexPath, const char* fragmentPath, const char* geometryPath);

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
		MeshComponent(std::shared_ptr<Light::Mesh> mesh);
		std::shared_ptr<Light::Mesh> mesh;
	};

	struct LightComponent : public Component
	{
		LightComponent() : m_lightColor({1.0, 1.0, 1.0}) {}
		LightComponent(glm::vec3 lightColor) : m_lightColor(lightColor) {}
		glm::vec3 m_lightColor;
		LightType m_lightType = LightType::Directional;
		float m_inner = 12.5;
		float m_outer = 17.5;
		float m_range  = 10.0;
	};

	struct CameraComponent : public Component
	{
		CameraComponent() = default;
		CameraComponent(glm::mat4 projectionMatrix) : m_camera(projectionMatrix) {}
		Camera m_camera;
	};
}

#endif // __COMPONENTS_HPP__
