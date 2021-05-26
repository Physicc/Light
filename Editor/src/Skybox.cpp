#include "Skybox.hpp"

Skybox::Skybox()
    : SceneObject({
                      {Light::ShaderDataType::Float3, "a_Position"}
                  })
{
    m_vao.reset(Light::VertexArray::create());

    float vertices[] = {
        //Front
        -1.0, -1.0, 1.0,
        1.0, -1.0, 1.0,
        1.0, 1.0, 1.0,
        -1.0, 1.0, 1.0,
        //Left
        -1.0, -1.0, 1.0,
        -1.0, 1.0, 1.0,
        -1.0, 1.0, -1.0,
        -1.0, -1.0, -1.0,
        //Right
        1.0, -1.0, 1.0,
        1.0, -1.0, -1.0,
        1.0, 1.0, -1.0,
        1.0, 1.0, 1.0,
        //Top
        -1.0, 1.0, 1.0,
        1.0, 1.0, 1.0,
        1.0, 1.0, -1.0,
        -1.0, 1.0, -1.0,
        //Bottom
        -1.0, -1.0, 1.0,
        -1.0, -1.0, -1.0,
        1.0, -1.0, -1.0,
        1.0, -1.0, 1.0,
        //Back
        -1.0, -1.0, -1.0,
        -1.0, 1.0, -1.0,
        1.0, 1.0, -1.0,
        1.0, -1.0, -1.0,
    };

    m_vbo.reset(Light::VertexBuffer::create(vertices, sizeof(vertices)));
    m_vbo->setLayout(m_layout);

    unsigned int indices[] = {
        0, 2, 1, 3, 2, 0,
        4, 6, 5, 7, 6, 4,
        8, 10, 9, 11, 10, 8,
        12, 14, 13, 15, 14, 12,
        16, 18, 17, 19, 18, 16,
        20, 22, 21, 23, 22, 20
    };

    m_ibo.reset(Light::IndexBuffer::create(indices,
                                           sizeof(indices)
                                               / sizeof(unsigned int)));

    m_vao->addVertexBuffer(m_vbo);
    m_vao->setIndexBuffer(m_ibo);

    m_cubemap.reset(Light::Cubemap::create("../Light/assets/cubemap"));
    m_shader = Light::Shader::create("../Light/assets/shaders/skybox.glsl");
    m_shader->bind();
    m_shader->setUniformInt("u_cubemap", 0);
}

void Skybox::render()
{
    m_cubemap->bind();
    Light::Renderer::submitSkybox(m_shader, m_vao);
}