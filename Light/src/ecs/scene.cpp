#include "ecs/scene.hpp"
#include "ecs/entity.hpp"
#include "ecs/components.hpp"
#include "core/timestep.hpp"
#include "core/input.hpp"
#include "input/keycodes.hpp"
#include "rendering/renderer.hpp"

Entity Scene::addEntity()
{
    return {m_registry.create(), this};
}

void Scene::update(Light::Timestep dt)
{
    auto view = m_registry.view<TransformComponent, Interactive>();
    for (auto entity : view) {
        auto& transform = view.get<TransformComponent>(entity);
        if(Light::Input::isKeyPressed(LIGHT_KEY_A))
        {
            transform.position.x -= 1.0 * dt.getSeconds();
        }
        else if(Light::Input::isKeyPressed(LIGHT_KEY_D))
        {
            transform.position.x += 1.0 * dt.getSeconds();
        }
        else if(Light::Input::isKeyPressed(LIGHT_KEY_W))
        {
            transform.position.z += 1.0 * dt.getSeconds();
        }
        else if(Light::Input::isKeyPressed(LIGHT_KEY_S))
        {
            transform.position.z -= 1.0 * dt.getSeconds();
        }
        else if(Light::Input::isKeyPressed(LIGHT_KEY_1))
        {
            transform.rotation.x += 2.0 * dt.getSeconds();
        }
        else if(Light::Input::isKeyPressed(LIGHT_KEY_2))
        {
            transform.rotation.x -= 2.0 * dt.getSeconds();
        }
        else if(Light::Input::isKeyPressed(LIGHT_KEY_3))
        {
            transform.scale += 0.2 * dt.getSeconds();
        }
        else if(Light::Input::isKeyPressed(LIGHT_KEY_4))
        {
            transform.scale -= 0.2 * dt.getSeconds();
        }
    }
}

void Scene::render()
{
    auto view1 = m_registry.view<ShaderComponent, MeshComponent, CubemapComponent>();
    for (auto& entity : view1) {
        auto [shader, mesh, cubemap] = view1.get(entity);
        cubemap.bind();
        Light::Renderer::submitSkybox(shader.shader, mesh.mesh);
    }
    auto view2 = m_registry.view<ShaderComponent, MeshComponent, TransformComponent>();
    for (auto& entity : view2) {
        auto [shader, mesh, transform] = view2.get(entity);
        Light::Renderer::submit(shader.shader, mesh.mesh, transform.getTransform());
    }
}