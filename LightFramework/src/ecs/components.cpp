#include "ecs/components.hpp"
#include "light/rendering/shader.hpp"
#include "light/rendering/texture.hpp"
#include "light/rendering/vertexarray.hpp"

namespace Light
{

	MeshRendererComponent::MeshRendererComponent(const char *path):
		shader(Light::Shader::create(path)) {}
	MeshRendererComponent::MeshRendererComponent(const char* vertexPath, const char* fragmentPath):
		shader(Light::Shader::create(vertexPath, fragmentPath)) {}
	MeshRendererComponent::MeshRendererComponent(const char* vertexPath, const char* fragmentPath, const char* geometryPath):
		shader(Light::Shader::create(vertexPath, fragmentPath, geometryPath)) {}

	MeshComponent::MeshComponent(std::shared_ptr<Light::Mesh> mesh)
		: mesh(mesh)
	{
	}
}
