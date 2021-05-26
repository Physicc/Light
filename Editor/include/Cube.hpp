#ifndef __CUBE_H__
#define __CUBE_H__

#include "SceneObject.hpp"

class Cube : SceneObject
{
    public:
        Cube(glm::vec3 position = glm::vec3(0, 0, 0),
             glm::vec3 rotation = glm::vec3(0, 0, 0),
             glm::vec3 scale = glm::vec3(0.5));

        void render() override;
        void onUpdate(Light::Timestep ts);
        void onEvent(Light::Event &e);
        inline glm::mat4 getTransform() { return m_transform; }

    private:
        glm::mat4 m_transform;
        glm::vec3 m_position;
        glm::vec3 m_rotation;
        glm::vec3 m_scale;

        void updateTransform();
};

#endif //__CUBE_H__
