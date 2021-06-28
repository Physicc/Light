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

struct TransformComponent {
    TransformComponent(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);
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

struct ShaderComponent {
    ShaderComponent(const char* path);
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

struct MeshComponent {
    MeshComponent(std::shared_ptr<Light::VertexBuffer> vertexBuffer, std::shared_ptr<Light::IndexBuffer> indexBuffer);
    std::shared_ptr<Light::VertexArray> mesh;
};

struct CubemapComponent {
    CubemapComponent(const char* path);
    inline void bind()
    {
        cubemap->bind();
    }
    std::shared_ptr<Light::Cubemap> cubemap;
};

struct Interactive {
    bool interactive = true;
};

#endif // __COMPONENTS_HPP__
