#ifndef __SCENEOBJECT_H__
#define __SCENEOBJECT_H__

#include "light.hpp"

//An abstract class which all objects in the Scene have to derive from
class SceneObject
{
    public:
        virtual void render() = 0;
        virtual void onUpdate(Light::Timestep ts) {};

    protected:
        SceneObject(Light::BufferLayout layout) : m_layout(std::move(layout)) {};

        Light::BufferLayout m_layout;
        std::shared_ptr<Light::VertexArray> m_vao;
        std::shared_ptr<Light::VertexBuffer> m_vbo;
        std::shared_ptr<Light::IndexBuffer> m_ibo;
        std::shared_ptr<Light::Shader> m_shader;
};
#endif // __SCENEOBJECT_H__