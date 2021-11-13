
#include "gui/scenepanel.hpp"

#include "physicsworld.hpp"
#include "imgui.h"
#include "light.hpp"
#include "core/entrypoint.hpp"

class MainLayer : public Light::Layer
{
public:
	MainLayer(): Light::Layer("MainLayer"),
					m_camera(45.0f, 1.6f / 0.9f, 0.001f, 100.0f)
	{
		Light::FramebufferSpec fbspec;
		fbspec.attachments = { 
			{ Light::FramebufferTextureFormat::RGBA8, Light::TextureWrap::CLAMP_TO_BORDER },
			{ Light::FramebufferTextureFormat::RED_INTEGER, Light::TextureWrap::CLAMP_TO_BORDER },
			{ Light::FramebufferTextureFormat::Depth, Light::TextureWrap::CLAMP_TO_BORDER }
		};
		fbspec.width = 1280;
		fbspec.height = 720;
		m_framebuffer = Light::Framebuffer::create(fbspec);

		Light::FramebufferSpec fbspecOutline;
		fbspecOutline.attachments = {
			{ Light::FramebufferTextureFormat::RED_INTEGER, Light::TextureWrap::CLAMP_TO_BORDER },
			{ Light::FramebufferTextureFormat::Depth, Light::TextureWrap::CLAMP_TO_BORDER }
		};
		fbspecOutline.width = 1280;
		fbspecOutline.height = 720;
		m_outlineFramebuffer = Light::Framebuffer::create(fbspecOutline);

		m_scene = std::make_shared<Light::Scene>();

		auto cube = m_scene->addEntity("Cube");
		cube.addComponent<Light::MeshRendererComponent>("assets/shaders/phong.glsl");

		auto floor = m_scene->addEntity("Floor");
		auto& floor_transform = floor.getComponent<Light::TransformComponent>();
		floor_transform.position = glm::vec3(0, -1, 0);
		floor_transform.scale = glm::vec3(2, 0.1, 2);
		floor.addComponent<Light::MeshRendererComponent>("assets/shaders/phong.glsl");

		auto light = m_scene->addEntity("Light");
		auto& light_transform = light.getComponent<Light::TransformComponent>();
		light_transform.position = glm::vec3(-1,2,1.5);
		light.addComponent<Light::LightComponent>();

		m_meshes = std::make_shared<Light::MeshLibrary>();

		addDefaultMeshes();

		cube.addComponent<Light::MeshComponent>(m_meshes->get("Cube"));
		floor.addComponent<Light::MeshComponent>(m_meshes->get("Cube"));

		m_scenePanel.setContext(m_scene);
		m_scenePanel.setMeshLibrary(m_meshes);
	}
	~MainLayer() = default;

	void onUpdate(Light::Timestep ts) override
	{
		if(m_resizeViewport)
		{
			m_camera.setViewportSize(int(m_viewportPanelSize.x), int(m_viewportPanelSize.y));
			m_framebuffer->resize(int(m_viewportPanelSize.x), int(m_viewportPanelSize.y));
			m_outlineFramebuffer->resize(int(m_viewportPanelSize.x), int(m_viewportPanelSize.y));
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
		Light::RenderCommand::setClearColor({0.5f, 0.1f, 0.1f, 1.0f});
		Light::RenderCommand::clear();

		m_framebuffer->clearAttachment(1, 0);


		Light::Renderer::beginScene(m_camera, m_camera.getViewMatrix());

		m_scene->render();

		Light::Renderer::endScene();

		auto[x, y] = ImGui::GetMousePos();

		glm::vec2 posRelativeToViewport = glm::vec2(x,y) - m_viewportPos;

		if(posRelativeToViewport.x >= 0 && posRelativeToViewport.y >= 0
			&& posRelativeToViewport.x < m_viewportPanelSize.x && posRelativeToViewport.y < m_viewportPanelSize.y)
		{
			int pixelData = m_framebuffer->readPixelInt(1, int(posRelativeToViewport.x), int(m_viewportPanelSize.y - posRelativeToViewport.y));

			m_hoveredEntity = pixelData == -1 ? Light::Entity() : Light::Entity((entt::entity)pixelData, m_scene.get());
		}

		m_framebuffer->unbind();

		auto entity = m_scenePanel.getSelectionContext();
		m_outlineFramebuffer->bind();
		Light::RenderCommand::setClearColor({0, 0, 0, 0});
		Light::RenderCommand::clear();
		m_scene->renderSelection(entity);
		m_outlineFramebuffer->unbind();

		m_framebuffer->bind();
		m_outlineFramebuffer->bindAttachmentTexture(0,0);
		m_scene->renderOutline();
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

	bool onMouseButtonPressed(Light::MouseButtonPressedEvent& e)
	{
		if(e.getButton() == LIGHT_MOUSE_BUTTON_LEFT)
		{
			m_scenePanel.setSelectionContext(m_hoveredEntity);
		}

		return false;
	}


	void onEvent(Light::Event& e) override
	{
		Light::EventDispatcher dispatcher(e);
		dispatcher.Dispatch<Light::WindowResizeEvent>(BIND_EVENT_FN(MainLayer::onWindowResize));
		dispatcher.Dispatch<Light::MouseButtonPressedEvent>(BIND_EVENT_FN(MainLayer::onMouseButtonPressed));
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
		m_viewportPos = { viewportPos.x, viewportPos.y };

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
		ImGui::Image(INT2VOIDP(m_framebuffer->getColorAttachmentRendererId(0)), viewPortPanelSize, ImVec2(0, 1), ImVec2(1, 0));
		
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

	void addDefaultMeshes()
	{
		m_meshes->add("None", std::vector<glm::vec3>(), std::vector<glm::vec4>(), std::vector<glm::vec3>(), std::vector<unsigned int>());

		std::vector<glm::vec3> vertices = {
			glm::vec3(-0.5f, -0.5f, 0.5f),
			glm::vec3(0.5f, -0.5f, 0.5f),
			glm::vec3(0.5f, 0.5f, 0.5f),
			glm::vec3(-0.5f, 0.5f, 0.5f),

			glm::vec3(-0.5f, -0.5f, 0.5f),
			glm::vec3(-0.5f, 0.5f, 0.5f),
			glm::vec3(-0.5f, 0.5f, -0.5f),
			glm::vec3(-0.5f, -0.5f, -0.5f),

			glm::vec3(0.5f, -0.5f, 0.5f),
			glm::vec3(0.5f, -0.5f, -0.5f),
			glm::vec3(0.5f, 0.5f, -0.5f),
			glm::vec3(0.5f, 0.5f, 0.5f),

			glm::vec3(-0.5f, 0.5f, 0.5f),
			glm::vec3(0.5f, 0.5f, 0.5f),
			glm::vec3(0.5f, 0.5f, -0.5f),
			glm::vec3(-0.5f, 0.5f, -0.5f),

			glm::vec3(-0.5f, -0.5f, 0.5f),
			glm::vec3(-0.5f, -0.5f, -0.5f),
			glm::vec3(0.5f, -0.5f, -0.5f),
			glm::vec3(0.5f, -0.5f, 0.5f),

			glm::vec3(-0.5f, -0.5f, -0.5f),
			glm::vec3(-0.5f, 0.5f, -0.5f),
			glm::vec3(0.5f, 0.5f, -0.5f),
			glm::vec3(0.5f, -0.5f, -0.5f)
		};

		std::vector<glm::vec4> color(24, {0.8f, 0.8f, 0.8f, 1.0f});

		std::vector<glm::vec3> normals = {
			glm::vec3(0.0f, 0.0f, 1.0f),
			glm::vec3(0.0f, 0.0f, 1.0f),
			glm::vec3(0.0f, 0.0f, 1.0f),
			glm::vec3(0.0f, 0.0f, 1.0f),

			glm::vec3(-1.0f, 0.0f, 0.0f),
			glm::vec3(-1.0f, 0.0f, 0.0f),
			glm::vec3(-1.0f, 0.0f, 0.0f),
			glm::vec3(-1.0f, 0.0f, 0.0f),

			glm::vec3(1.0f, 0.0f, 0.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),

			glm::vec3(0.0f, 1.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f),

			glm::vec3(0.0f, -1.0f, 0.0f),
			glm::vec3(0.0f, -1.0f, 0.0f),
			glm::vec3(0.0f, -1.0f, 0.0f),
			glm::vec3(0.0f, -1.0f, 0.0f),

			glm::vec3(0.0f, 0.0f, -1.0f),
			glm::vec3(0.0f, 0.0f, -1.0f),
			glm::vec3(0.0f, 0.0f, -1.0f),
			glm::vec3(0.0f, 0.0f, -1.0f)
		};

		std::vector<unsigned int> indices = {
			0, 1, 2, 2, 3, 0,
			4, 5, 6, 6, 7, 4,
			8, 9, 10, 10, 11, 8,
			12, 13, 14, 14, 15, 12,
			16, 17, 18, 18, 19, 16,
			20, 21, 22, 22, 23, 20
		};

		m_meshes->add("Cube", vertices, color, normals, indices);
	}

private:
	std::shared_ptr<Light::MeshLibrary> m_meshes;

	Light::EditorCamera m_camera;

	std::shared_ptr<Light::Scene> m_scene;
	Light::ScenePanel m_scenePanel;

	Light::Entity m_hoveredEntity;

	std::shared_ptr<Light::Framebuffer> m_framebuffer;
	std::shared_ptr<Light::Framebuffer> m_outlineFramebuffer;
	
	glm::vec2 m_viewportPanelSize;
	glm::vec2 m_viewportPos;

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