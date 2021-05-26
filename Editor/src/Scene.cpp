#include "Scene.hpp"

void Scene::addObject(SceneObject* obj, bool update, bool event)
{
    if (update) {
        m_updateSceneObjects.push_back(std::shared_ptr<SceneObject>(obj));
    }
    if (event) {
        m_eventSceneObjects.push_back(std::shared_ptr<SceneObject>(obj));
    }
    m_sceneObjects.push_back(std::shared_ptr<SceneObject>(obj));
}

void Scene::onUpdate(Light::Timestep ts)
{
    m_camera.onUpdate(ts);
    for (auto object: m_updateSceneObjects)
    {
        object->onUpdate(ts);
    }
}

void Scene::onEvent(Light::Event& e) {
    m_camera.onEvent(e);
    for (auto object: m_eventSceneObjects)
    {
        object->onEvent(e);
    }
}

void Scene::render()
{
    for (auto object: m_sceneObjects)
    {
        object->render();
    }
}

void Scene::setViewportSize(float x, float y)
{
    m_camera.setViewportSize(x, y);
}
