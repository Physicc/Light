#include "light.hpp"
#include "core/entrypoint.hpp"

#include "gui/scenepanel.hpp"

#include "imgui.h"

class MainLayer : public Light::Layer
{
public:
	MainLayer(): Light::Layer("MainLayer"),
					m_camera(45.0f, 1.6f / 0.9f, 0.001f, 100.0f)
					// m_lightPos(-1, 2, 1.5)
	{
		Light::FramebufferSpec fbspec;
		fbspec.width = 1280;
		fbspec.height = 720;
		m_framebuffer = Light::Framebuffer::create(fbspec);

		m_scene = std::make_shared<Light::Scene>();

		auto cube = m_scene->addEntity("Cube");
		cube.addComponent<Light::MeshRendererComponent>("../Editor/assets/shaders/phong.glsl");

		auto floor = m_scene->addEntity("Floor");
		auto& floor_transform = floor.getComponent<Light::TransformComponent>();
		floor_transform.position = glm::vec3(0, -1, 0);
		floor_transform.scale = glm::vec3(2, 0.1, 2);
		floor.addComponent<Light::MeshRendererComponent>("../Editor/assets/shaders/phong.glsl");

		m_light = m_scene->addEntity("Light");
		auto& light_transform = m_light.getComponent<Light::TransformComponent>();
		light_transform.position = glm::vec3(-1,2,1.5);
		m_light.addComponent<Light::LightComponent>();

		Light::BufferLayout layout({
				{ Light::ShaderDataType::Float3, "a_Position" },
				{ Light::ShaderDataType::Float4, "a_Color" },
				{ Light::ShaderDataType::Float3, "a_Normal" }
			});

		float vertices[] = {
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

		std::shared_ptr<Light::VertexBuffer> vbo(Light::VertexBuffer::create(vertices, sizeof(vertices)));
		vbo->setLayout(layout);

		unsigned int indices[] = { 
			0, 1, 2, 2, 3, 0,
			4, 5, 6, 6, 7, 4,
			8, 9, 10, 10, 11, 8,
			12, 13, 14, 14, 15, 12,
			16, 17, 18, 18, 19, 16,
			20, 21, 22, 22, 23, 20
		};

		std::shared_ptr<Light::IndexBuffer> ibo(Light::IndexBuffer::create(indices, sizeof(indices) / sizeof(unsigned int)));

		cube.addComponent<Light::MeshComponent>(vbo, ibo);
		floor.addComponent<Light::MeshComponent>(vbo, ibo);

		m_scenePanel.setContext(m_scene);
	}
	~MainLayer() = default;

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

		m_framebuffer->bind();

		Light::RenderCommand::setClearColor({0.2f,0.2f,0.2f,1.0f});
		Light::RenderCommand::clear();

		Light::Renderer::beginScene(m_camera, m_camera.getViewMatrix());

		m_scene->render();

		Light::Renderer::endScene();

		m_framebuffer->unbind();
	}

	bool onWindowResize(Light::WindowResizeEvent& e)
	{
		auto[width, height] = e.getSize();
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
		dispatcher.Dispatch<Light::WindowResizeEvent>(BIND_EVENT_FN(MainLayer::onWindowResize));
		m_camera.onEvent(e);
	}

	void onImguiRender() override
	{
		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());


		// Main Menu Bar
		if(ImGui::BeginMainMenuBar())
		{
			if(ImGui::BeginMenu("File"))
			{
				if(ImGui::MenuItem("Exit")) Light::Application::get().close();
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}

		// Viewport
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

		// Scene Hierarchy Panel
		m_scenePanel.onImguiRender();

		// Camera controls panel
		ImGui::Begin("Camera Controls");
		ImGui::Text("Left Alt + LMB to Orbit");
		ImGui::Text("Left Alt + MMB to Pan");
		ImGui::Text("Left Alt + RMB to Zoom");
		ImGui::Text("Scroll to Zoom");
		ImGui::End();

		// Perf stats
		ImGui::Begin("Performance Statistics");
		ImGui::Text("MSPF: %0.2f\nSPF: %0.4f\nFPS: %d",
					m_lastTime / m_lastFrameCount,
					m_lastTime * 0.001f / m_lastFrameCount,
					int(m_lastFrameCount * 1000 / m_lastTime));
		ImGui::End();

	}

private:
	Light::EditorCamera m_camera;

	std::shared_ptr<Light::Scene> m_scene;
	Light::Entity m_light;

	Light::ScenePanel m_scenePanel;

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
		pushLayer(new MainLayer());
	}
	~Editor() = default;
};

Light::Application* Light::createApplication()
{
	return new Editor();
}