#ifndef __SCENE_H__
#define __SCENE_H__

#include "light.hpp"
#include "SceneObject.hpp"
#include "Cube.hpp"
#include "Skybox.hpp"

class Scene
{
    public:
//        m_camera(45.0f, 1.6f / 0.9f, 0.001f, 100.0f),
//        m_lightPos(-1, 2, 1.5),
//        m_floor(glm::vec3(0, -1, 0), glm::vec3(0), glm::vec3(2, 0.1, 2))

        Scene(float fovy, float aspectRatio, float near, float far,
              float lightPos_x, float lightPos_y, float lightPos_z)//,
//              glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
            : m_camera(fovy, aspectRatio, near, far),
              m_lightPos(lightPos_x, lightPos_y, lightPos_z)//,
//              m_floor(position, rotation, scale)
        {}

        void onUpdate(Light::Timestep ts);

        void render();

        void setViewportSize(float x, float y);

        glm::vec3 &getLightPos()
        {
            return m_lightPos;
        }

        const Light::EditorCamera &getCamera() const
        {
            return m_camera;
        }

        void addObject(SceneObject obj, bool update)
        {
            if (update) {
                m_updateSceneObjects.push_back(&obj);
            }
            m_sceneObjects.push_back(&obj);
        }

    private:
        std::vector<SceneObject *> m_sceneObjects;
        //Holds pointers to every object in the Scene
        std::vector<SceneObject *> m_updateSceneObjects;
        //Holds pointers to every object that needs to have onUpdate run
        //in the Scene
        Light::EditorCamera m_camera;
//        Cube m_cube;
//        Cube m_floor;
//        Skybox m_skybox;
        glm::vec3 m_lightPos;

};

#endif //__SCENE_H__
