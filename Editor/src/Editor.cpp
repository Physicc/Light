#include "light.hpp"
#include "core/entrypoint.hpp"

#include "gui/scenepanel.hpp"

#include "Objects.hpp"
#include "physicsworld.hpp"
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

		if(m_viewportFocused)
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
		static int s_stats_corner = 0;
		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

		// Demo
		// ImGui::ShowDemoWindow();

		// Main Menu Bar
		if(ImGui::BeginMainMenuBar())
		{
			if(ImGui::BeginMenu("File"))
			{
				if(ImGui::MenuItem("Exit")) Light::Application::get().close();
				ImGui::EndMenu();
			}
			if(ImGui::BeginMenu("Settings"))
			{
				if(ImGui::MenuItem("Show stats", NULL, s_stats_corner != -1))
					s_stats_corner = (s_stats_corner == -1) ? 0 : -1;
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}

		// Viewport
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0,0));
		ImGui::Begin("Viewport");

		ImVec2 viewportPos = ImGui::GetCursorScreenPos();

		bool viewportDocked = ImGui::IsWindowDocked();

		Light::Application::get().getImguiLayer()->blockFocusEvents(!ImGui::IsWindowFocused());
		Light::Application::get().getImguiLayer()->blockHoverEvents(!ImGui::IsWindowHovered());

		m_viewportFocused = ImGui::IsWindowFocused();

		ImVec2 viewPortPanelSize = ImGui::GetContentRegionAvail();
		if(m_viewportPanelSize != *(glm::vec2*)&viewPortPanelSize)
		{
			m_resizeViewport = true;
			m_viewportPanelSize = glm::vec2(viewPortPanelSize.x, viewPortPanelSize.y);
		}
		ImGui::Image(INT2VOIDP(m_framebuffer->getColorAttachmentRendererId()), viewPortPanelSize, ImVec2(0, 1), ImVec2(1, 0));
		
		ImGui::End();
		ImGui::PopStyleVar();

		// Perf Stats
		{
			ImGuiIO& io = ImGui::GetIO();
			ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
			if (s_stats_corner != -1)
			{
				const float PAD = 10.0f;
				ImVec2 work_pos = viewportPos;
				ImVec2 work_size = viewPortPanelSize;
				ImVec2 window_pos, window_pos_pivot;
				window_pos.x = (s_stats_corner & 1) ? (work_pos.x + work_size.x - PAD) : (work_pos.x + PAD);
				window_pos.y = (s_stats_corner & 2) ? (work_pos.y + work_size.y - PAD) : (work_pos.y + PAD);
				window_pos_pivot.x = (s_stats_corner & 1) ? 1.0f : 0.0f;
				window_pos_pivot.y = (s_stats_corner & 2) ? 1.0f : 0.0f;
				ImGui::SetNextWindowViewport(ImGui::GetMainViewport()->ID);

				ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
				if(viewportDocked)
				{
					ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
					window_flags |= ImGuiWindowFlags_NoMove;
				}
				if (ImGui::Begin("Performance Statistics", NULL, window_flags))
				{
					ImGui::Text("MSPF: %0.2f\nSPF: %0.4f\nFPS: %d",
						m_lastTime / m_lastFrameCount,
						m_lastTime * 0.001f / m_lastFrameCount,
						int(m_lastFrameCount * 1000 / m_lastTime));
					ImGui::Separator();
					ImGui::Text("(Right-Click to change position)");
					if (ImGui::BeginPopupContextWindow())
					{
						if (ImGui::MenuItem("Hide", NULL, s_stats_corner == -1)) s_stats_corner = -1;
						if(viewportDocked)
						{
							if (ImGui::MenuItem("Top-left",     NULL, s_stats_corner == 0)) s_stats_corner = 0;
							if (ImGui::MenuItem("Top-right",    NULL, s_stats_corner == 1)) s_stats_corner = 1;
							if (ImGui::MenuItem("Bottom-left",  NULL, s_stats_corner == 2)) s_stats_corner = 2;
							if (ImGui::MenuItem("Bottom-right", NULL, s_stats_corner == 3)) s_stats_corner = 3;
						}
						ImGui::EndPopup();
					}
				}
				ImGui::End();
			}
		}


		// Scene Hierarchy Panel
		m_scenePanel.onImguiRender();

	}

private:
	Light::EditorCamera m_camera;

	std::shared_ptr<Light::Scene> m_scene;
	Light::Entity m_light;

	Light::ScenePanel m_scenePanel;

	std::shared_ptr<Light::Framebuffer> m_framebuffer;
	glm::vec2 m_viewportPanelSize;
	bool m_resizeViewport = false;
	bool m_viewportFocused = false;
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