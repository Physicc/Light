#include "light.hpp"
#include "core/entrypoint.hpp"

#include "imgui.h"

#include "scene.hpp"
#include "entity.hpp"
#include "components.hpp"

class ExampleLayer : public Light::Layer
{
public:
	ExampleLayer(): Light::Layer("TestLayer"),
                    m_camera(45.0f, 1.6f / 0.9f, 0.001f, 100.0f),
                    m_lightPos(-1, 2, 1.5)
	{
	    Light::FramebufferSpec fbspec{1280, 720};
        m_framebuffer = Light::Framebuffer::create(fbspec);
        // =================
        // Add cube to scene
        // =================
        auto cube = m_scene.addEntity();
        cube.addComponent<TransformComponent>(glm::vec3(0,0,0), glm::vec3(0,0,0), glm::vec3(0.5));
        cube.addComponent<ShaderComponent>("../Light/assets/shaders/phong.glsl");
        float cubeVertices[] = {
                //Front
                -0.5, -0.5, 0.5, 0.8, 0.8, 0.8, 1.0, 0, 0, 1.0,
                0.5, -0.5, 0.5, 0.8, 0.8, 0.8, 1.0, 0, 0, 1.0,
                0.5, 0.5, 0.5, 0.8, 0.8, 0.8, 1.0, 0, 0, 1.0,
                -0.5, 0.5, 0.5, 0.8, 0.8, 0.8, 1.0, 0, 0, 1.0,
                //Left
                -0.5, -0.5, 0.5, 0.8, 0.8, 0.8, 1.0, -1.0, 0, 0,
                -0.5, 0.5, 0.5, 0.8, 0.8, 0.8, 1.0, -1.0, 0, 0,
                -0.5, 0.5, -0.5, 0.8, 0.8, 0.8, 1.0, -1.0, 0, 0,
                -0.5, -0.5, -0.5, 0.8, 0.8, 0.8, 1.0, -1.0, 0, 0,
                //Right
                0.5, -0.5, 0.5, 0.8, 0.8, 0.8, 1.0, 1.0, 0, 0,
                0.5, -0.5, -0.5, 0.8, 0.8, 0.8, 1.0, 1.0, 0, 0,
                0.5, 0.5, -0.5, 0.8, 0.8, 0.8, 1.0, 1.0, 0, 0,
                0.5, 0.5, 0.5, 0.8, 0.8, 0.8, 1.0, 1.0, 0, 0,
                //Top
                -0.5, 0.5, 0.5, 0.8, 0.8, 0.8, 1.0, 0, 1.0, 0,
                0.5, 0.5, 0.5, 0.8, 0.8, 0.8, 1.0, 0, 1.0, 0,
                0.5, 0.5, -0.5, 0.8, 0.8, 0.8, 1.0, 0, 1.0, 0,
                -0.5, 0.5, -0.5, 0.8, 0.8, 0.8, 1.0, 0, 1.0, 0,
                //Bottom
                -0.5, -0.5, 0.5, 0.8, 0.8, 0.8, 1.0, 0, -1.0, 0,
                -0.5, -0.5, -0.5, 0.8, 0.8, 0.8, 1.0, 0, -1.0, 0,
                0.5, -0.5, -0.5, 0.8, 0.8, 0.8, 1.0, 0, -1.0, 0,
                0.5, -0.5, 0.5, 0.8, 0.8, 0.8, 1.0, 0, -1.0, 0,
                //Back
                -0.5, -0.5, -0.5, 0.8, 0.8, 0.8, 1.0, 0, 0, -1.0,
                -0.5, 0.5, -0.5, 0.8, 0.8, 0.8, 1.0, 0, 0, -1.0,
                0.5, 0.5, -0.5, 0.8, 0.8, 0.8, 1.0, 0, 0, -1.0,
                0.5, -0.5, -0.5, 0.8, 0.8, 0.8, 1.0, 0, 0, -1.0,
        };
        std::shared_ptr<Light::VertexBuffer> vertexBuffer(Light::VertexBuffer::create(cubeVertices, sizeof(cubeVertices)));
        vertexBuffer->setLayout({{ Light::ShaderDataType::Float3, "a_Position" },
                                 { Light::ShaderDataType::Float4, "a_Color" },
                                 { Light::ShaderDataType::Float3, "a_Normal" }});
        unsigned int cubeIndices[] = {
                0, 1, 2, 2, 3, 0,
                4, 5, 6, 6, 7, 4,
                8, 9, 10, 10, 11, 8,
                12, 13, 14, 14, 15, 12,
                16, 17, 18, 18, 19, 16,
                20, 21, 22, 22, 23, 20
        };
        std::shared_ptr<Light::IndexBuffer> indexBuffer(Light::IndexBuffer::create(cubeIndices, sizeof(cubeIndices) / sizeof(unsigned int)));
        cube.addComponent<VertexArrayComponent>(vertexBuffer, indexBuffer);
        cube.addComponent<Interactive>();
        // ==================
        // Add floor to scene
        // ==================
        auto floor = m_scene.addEntity();
        floor.addComponent<TransformComponent>(glm::vec3(0, -1, 0), glm::vec3(0), glm::vec3(2, 0.1, 2));
        floor.addComponent<ShaderComponent>("../Light/assets/shaders/phong.glsl");
        floor.addComponent<VertexArrayComponent>(vertexBuffer, indexBuffer);
        // ===================
        // Add skybox to scene
        // ===================
        auto skybox = m_scene.addEntity();
        auto& cubemap = skybox.addComponent<CubemapComponent>("../Light/assets/cubemap");
        auto& shader = skybox.addComponent<ShaderComponent>("../Light/assets/shaders/skybox.glsl");
        shader.bind();
        shader.setUniformInt("u_cubemap", 0);
        float skyboxVertices[] = {
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
        vertexBuffer.reset(Light::VertexBuffer::create(skyboxVertices, sizeof(skyboxVertices)));
        vertexBuffer->setLayout({{ Light::ShaderDataType::Float3, "a_Position" }});
        unsigned int skyboxIndices[] = {
                0, 2, 1, 3, 2, 0,
                4, 6, 5, 7, 6, 4,
                8, 10, 9, 11, 10, 8,
                12, 14, 13, 15, 14, 12,
                16, 18, 17, 19, 18, 16,
                20, 22, 21, 23, 22, 20
        };
        indexBuffer.reset(Light::IndexBuffer::create(skyboxIndices, sizeof(skyboxIndices) / sizeof(unsigned int)));
        skybox.addComponent<VertexArrayComponent>(vertexBuffer, indexBuffer);
	}
	~ExampleLayer() = default;

	void onUpdate(Light::Timestep ts) override
	{
		if(m_resizeViewport)
		{
			m_camera.setViewportSize(m_viewportPanelSize.x, m_viewportPanelSize.y);
			m_framebuffer->resize(m_viewportPanelSize.x, m_viewportPanelSize.y);
            m_resizeViewport = false;
		}
		m_frameCount++;
        m_time += ts.getMilliSeconds();
		if(m_time >= 500.0f)
		{
            m_lastTime = m_time;
            m_lastFrameCount = m_frameCount;
            m_time = 0.0f;
            m_frameCount = 0;
		}

		m_camera.onUpdate(ts);
		m_scene.update(ts);

		m_framebuffer->bind();
		Light::RenderCommand::setClearColor({0.2f,0.2f,0.2f,1.0f});
		Light::RenderCommand::clear();
		Light::Renderer::beginScene(m_camera, m_lightPos);
		m_scene.render();
		Light::Renderer::endScene();
		m_framebuffer->unbind();
	}

	bool onWindowResize(Light::WindowResizeEvent& e)
	{
		auto [width, height] = e.getSize();
        if(width == 0 || height == 0)
        {
            return false;
        }
		m_camera.setViewportSize(width, height);
		return false;
	}

	void onEvent(Light::Event& e) override
	{
		Light::EventDispatcher dispatcher(e);
		dispatcher.Dispatch<Light::WindowResizeEvent>(BIND_EVENT_FN(ExampleLayer::onWindowResize));
		m_camera.onEvent(e);
	}

	void onImguiRender() override
	{
		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());


		if(ImGui::BeginMainMenuBar())
		{
			if(ImGui::BeginMenu("File"))
			{
				if(ImGui::MenuItem("Exit")) Light::Application::get().close();
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0,0));
		ImGui::Begin("Viewport");

		Light::Application::get().getImguiLayer()->blockFocusEvents(!ImGui::IsWindowFocused());
		Light::Application::get().getImguiLayer()->blockHoverEvents(!ImGui::IsWindowHovered());

		ImVec2 panelSize = ImGui::GetContentRegionAvail();
		if(m_viewportPanelSize != *(glm::vec2*)&panelSize)
		{
            m_resizeViewport = true;
            m_viewportPanelSize = glm::vec2(panelSize.x, panelSize.y);
		}
		ImGui::Image(INT2VOIDP(m_framebuffer->getColorAttachmentRendererId()), panelSize, ImVec2(0, 1), ImVec2(1, 0));
		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::Begin("Scene Settings");
		ImGui::DragFloat3("Light Position", &(m_lightPos.x), 0.01f);
		ImGui::End();

		ImGui::Begin("Camera Controls");
		ImGui::Text("Left Alt + LMB to Orbit");
		ImGui::Text("Left Alt + MMB to Pan");
		ImGui::Text("Left Alt + RMB to Zoom");
		ImGui::Text("Scroll to Zoom");
		ImGui::End();

		ImGui::Begin("Performance Statistics");
		ImGui::Text("MSPF: %0.2f\nSPF: %0.4f\nFPS: %d",
                    m_lastTime / m_lastFrameCount,
                    m_lastTime * 0.001f / m_lastFrameCount,
                    int(m_lastFrameCount * 1000 / m_lastTime));
		ImGui::End();

	}

private:
    Scene m_scene;
	Light::EditorCamera m_camera;
	glm::vec3 m_lightPos;
	std::shared_ptr<Light::Framebuffer> m_framebuffer;
	glm::vec2 m_viewportPanelSize;
	bool m_resizeViewport = false;
	float m_time = 0.0f;
	int m_frameCount = 0;
	float m_lastTime = 0.0f;
	int m_lastFrameCount = 0;
};

class Editor : public Light::Application
{
public:
	Editor()
	{
		pushLayer(new ExampleLayer());
	}
	~Editor() = default;
};

Light::Application* Light::createApplication()
{
	return new Editor();
}