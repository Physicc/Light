#include "Scene.hpp"

Scene::Scene(std::vector<float> cam_param, std::vector<float> light_param, std::vector<glm::vec3> floor_param) 
        : camera(cam_param[0],cam_param[1],cam_param[2],cam_param[3]),
        lightPos(light_param[0],light_param[1],light_param[2]), 
        floor(floor_param[0],floor_param[1],floor_param[2])
{

}

void Scene::object_init(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
{
    Cube cube(position,rotation,scale);
    objects.push_back(cube);
}

void Scene::onUpdate(Light::Timestep ts)
{
    camera.onUpdate(ts);
    for(auto object : objects) {object.onUpdate(ts);}
}

void Scene::render()
{
    Light::RenderCommand::setClearColor({0.2f,0.2f,0.2f,1.0f});
    Light::RenderCommand::clear();

    Light::Renderer::beginScene(camera, lightPos);
    
    skybox.render();
    for(auto object : objects) 
    {
        object.render();
    }
    floor.render();

    Light::Renderer::endScene();
}

void Scene::camResize(float x, float y)
{
    camera.setViewportSize(x,y);
}

void Scene::onEvent(Light::Event& e)
{
    camera.onEvent(e);
    for(auto object : objects) 
    {
        object.onEvent(e);
    }
}

std::vector<float> Scene::getLightPos()
{
    return {lightPos.x, lightPos.y, lightPos.z};
}