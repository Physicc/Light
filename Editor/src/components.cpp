#include "components.hpp"

TransformComponent::TransformComponent(const glm::vec3 &position, const glm::vec3 &rotation, const glm::vec3 &scale)
    : m_position(position), m_rotation(rotation), m_scale(scale) {}

ShaderComponent::ShaderComponent(const char *path) : m_shader(Light::Shader::create(path)) {}

VertexArrayComponent::VertexArrayComponent(std::shared_ptr<Light::VertexBuffer> vertexBuffer, std::shared_ptr<Light::IndexBuffer> indexBuffer)
    : m_vertexArray(Light::VertexArray::create()) {
    m_vertexArray->addVertexBuffer(vertexBuffer);
    m_vertexArray->setIndexBuffer(indexBuffer);
}

CubemapComponent::CubemapComponent(const char* path) : m_cubemap(Light::Cubemap::create(path)){}