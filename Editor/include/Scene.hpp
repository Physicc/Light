#ifndef __SCENE_H__
#define __SCENE_H__

#include "light.hpp"
#include "SceneObject.hpp"
#include "Cube.hpp"
#include "Skybox.hpp"

class Scene
{
    public:
        Scene(float fovy, float aspectRatio, float near, float far,
              float lightPos_x, float lightPos_y, float lightPos_z)
            : m_camera(fovy, aspectRatio, near, far),
              m_lightPos(lightPos_x, lightPos_y, lightPos_z)
        {}

        void addObject(SceneObject *obj, bool update, bool event);
        void onUpdate(Light::Timestep ts);
        void onEvent(Light::Event &e);
        void render();
        void setViewportSize(float x, float y);

        glm::vec3 &getLightPos()
        {
            return m_lightPos;
        }

        Light::EditorCamera &getCamera()
        {
            return m_camera;
        }

    private:
        std::vector<std::shared_ptr<SceneObject>> m_sceneObjects;
        //Holds pointers to every object in the Scene
        std::vector<std::shared_ptr<SceneObject>> m_updateSceneObjects;
        //Holds pointers to every object that needs to have onUpdate run
        //in the Scene
        std::vector<std::shared_ptr<SceneObject>> m_eventSceneObjects;
        //Holds pointers to every object that needs to have onEvent run
        //in the Scene
        Light::EditorCamera m_camera;
        glm::vec3 m_lightPos;

};

#endif //__SCENE_H__
