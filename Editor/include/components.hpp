#ifndef __COMPONENTS_HPP__
#define __COMPONENTS_HPP__

#include "light.hpp"
#include <memory>
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

struct TransformComponent {
    TransformComponent(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);
    inline glm::mat4 getTransform() const {
       return glm::translate(glm::mat4(1.0f), m_position)
               * glm::toMat4(glm::quat(m_rotation))
               * glm::scale(glm::mat4(1.0f), m_scale);
    }
    glm::vec3 m_position;
    glm::vec3 m_rotation;
    glm::vec3 m_scale;
};

struct ShaderComponent {
    ShaderComponent(const char* path);
    std::shared_ptr<Light::Shader> m_shader;
};

struct VertexArrayComponent {
    VertexArrayComponent(std::shared_ptr<Light::VertexBuffer> vertexBuffer, std::shared_ptr<Light::IndexBuffer> indexBuffer);
    std::shared_ptr<Light::VertexArray> m_vertexArray;
};

struct CubemapComponent {
    CubemapComponent(const char* path);
    std::shared_ptr<Light::Cubemap> m_cubemap;
};

#endif // __COMPONENTS_HPP__
