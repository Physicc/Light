#include "components.hpp"

TransformComponent::TransformComponent(const glm::vec3 &position, const glm::vec3 &rotation, const glm::vec3 &scale)
        : position(position), rotation(rotation), scale(scale) {}

ShaderComponent::ShaderComponent(const char *path) : shader(Light::Shader::create(path)) {}

VertexArrayComponent::VertexArrayComponent(std::shared_ptr<Light::VertexBuffer> vertexBuffer, std::shared_ptr<Light::IndexBuffer> indexBuffer)
        : vertexArray(Light::VertexArray::create()) {
    vertexArray->addVertexBuffer(vertexBuffer);
    vertexArray->setIndexBuffer(indexBuffer);
}

CubemapComponent::CubemapComponent(const char* path) : cubemap(Light::Cubemap::create(path)) {}