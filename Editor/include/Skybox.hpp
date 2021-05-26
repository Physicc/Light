#ifndef __SKYBOX_H__
#define __SKYBOX_H__

#include "SceneObject.hpp"

class Skybox : SceneObject
{
    public:
        Skybox();
        void render();

    private:
        Light::BufferLayout m_layout;
        std::shared_ptr<Light::VertexArray> m_vao;
        std::shared_ptr<Light::VertexBuffer> m_vbo;
        std::shared_ptr<Light::IndexBuffer> m_ibo;
        std::shared_ptr<Light::Shader> m_shader;
        std::shared_ptr<Light::Cubemap> m_cubemap;
};

#endif //__SKYBOX_H__
