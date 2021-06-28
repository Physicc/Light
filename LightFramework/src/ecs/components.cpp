#include "ecs/components.hpp"
#include "light/rendering/shader.hpp"
#include "light/rendering/texture.hpp"
#include "light/rendering/vertexarray.hpp"


TransformComponent::TransformComponent(const glm::vec3 &position, const glm::vec3 &rotation, const glm::vec3 &scale)
        : position(position), rotation(rotation), scale(scale) {}

ShaderComponent::ShaderComponent(const char *path) : shader(Light::Shader::create(path)) {}

MeshComponent::MeshComponent(std::shared_ptr<Light::VertexBuffer> vertexBuffer, std::shared_ptr<Light::IndexBuffer> indexBuffer)
        : mesh(Light::VertexArray::create())
{
    mesh->addVertexBuffer(vertexBuffer);
    mesh->setIndexBuffer(indexBuffer);
}

CubemapComponent::CubemapComponent(const char* path) : cubemap(Light::Cubemap::create(path)) {}