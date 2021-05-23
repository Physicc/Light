#ifndef __SCENE_H__
#define __SCENE_H__

#include "light.hpp"
#include "Objects.hpp"

class Scene
{
public:
    Scene(std::vector<float> cam_param, std::vector<float> light_param, std::vector<glm::vec3> floor_param);
    
    void object_init(glm::vec3 position = glm::vec3(0,0,0), 
    glm::vec3 rotation = glm::vec3(0,0,0), 
    glm::vec3 scale = glm::vec3(0.5)); 

    void onUpdate(Light::Timestep ts);
    void onEvent(Light::Event& e);
    void render();
    void camResize(float x, float y);
    
    std::vector<float> getLightPos();       //{x,y,z}

private:
    Light::EditorCamera camera;
    Cube floor;
    Skybox skybox;
    glm::vec3 lightPos;
    std::vector<Cube> objects;

};

#endif //__SCENE_H__