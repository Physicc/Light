#include "ecs/components.hpp"
#include "light/rendering/shader.hpp"
#include "light/rendering/texture.hpp"
#include "light/rendering/vertexarray.hpp"

namespace Light
{

	MeshRendererComponent::MeshRendererComponent(const char *path) : shader(Light::Shader::create(path)) {}

	MeshComponent::MeshComponent(std::shared_ptr<Light::Mesh> mesh)
		: mesh(mesh)
	{
	}
}